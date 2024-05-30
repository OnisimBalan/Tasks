#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sqlite3.h>
#include <ctime>

#define PORT 8080

using namespace std;

void handleClientRequest(int client_socket, sqlite3 *db)
{
  char buffer[1024] = {0};
  read(client_socket, buffer, 1024);
  string option(buffer);

  if (option == "1" || option == "2")
  {
    // Option to convert files
    int filenameLength;
    read(client_socket, &filenameLength, sizeof(filenameLength));
    read(client_socket, buffer, filenameLength);
    string filename(buffer);

    cout << "Received request to convert file: " << filename << endl;

    // Receive the file content
    ofstream outFile(filename, ios::binary);
    int bytesReceived;
    while ((bytesReceived = read(client_socket, buffer, 1024)) > 0)
    {
      outFile.write(buffer, bytesReceived);
    }
    outFile.close();

    // Here you can implement the file conversion logic
    // For now, let's just print a message indicating the file received
    cout << "File received: " << filename << endl;
  }
  else if (option == "3")
  {
    // Option to request database data
    string response;
    string sql = "SELECT * FROM files;";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
      cerr << "Failed to execute SQL query" << endl;
      return;
    }

    // Construct the response with data from the database
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
      string filename = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
      string timestamp = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));
      response += filename + ";" + timestamp + "\n";
    }

    sqlite3_finalize(stmt);

    // Send the data to the client
    send(client_socket, response.c_str(), response.size(), 0);
  }
  else
  {
    cout << "Received unknown request from client: " << option << endl;
  }
}

int main()
{
  int server_fd, new_socket;
  struct sockaddr_in address;
  int opt = 1;
  int addrlen = sizeof(address);

  // Initialize SQLite
  sqlite3 *db;
  int rc = sqlite3_open("files.db", &db);
  if (rc)
  {
    cerr << "Can't open database: " << sqlite3_errmsg(db) << endl;
    return 1;
  }

  // Create table if it doesn't exist
  const char *sql = "CREATE TABLE IF NOT EXISTS files (id INTEGER PRIMARY KEY AUTOINCREMENT, filename TEXT NOT NULL, timestamp TEXT NOT NULL);";
  char *errMsg = nullptr;
  rc = sqlite3_exec(db, sql, nullptr, nullptr, &errMsg);
  if (rc != SQLITE_OK)
  {
    cerr << "SQL error: " << errMsg << endl;
    sqlite3_free(errMsg);
    return 1;
  }

  // Create socket
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
  {
    perror("socket failed");
    exit(EXIT_FAILURE);
  }

  // Set socket options
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
  {
    perror("setsockopt");
    exit(EXIT_FAILURE);
  }
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT);

  // Bind socket to port
  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
  {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }
  if (listen(server_fd, 3) < 0)
  {
    perror("listen");
    exit(EXIT_FAILURE);
  }

  while (true)
  {
    cout << "Waiting for a connection..." << endl;
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
    {
      perror("accept");
      exit(EXIT_FAILURE);
    }

    // Handle client request
    handleClientRequest(new_socket, db);

    close(new_socket);
  }

  sqlite3_close(db);
  return 0;
}
