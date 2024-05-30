#include <iostream>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fstream>

#define PORT 8080

using namespace std;

void sendFile(const string &filename, int option) {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        cout << "Socket creation error" << endl;
        return;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        cout << "Invalid address/ Address not supported" << endl;
        return;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        cout << "Connection Failed" << endl;
        return;
    }

    // Trimite opțiunea către server
    string optionStr = to_string(option);
    send(sock, optionStr.c_str(), optionStr.size(), 0);

    // Trimite numele fișierului
    send(sock, filename.c_str(), filename.size(), 0);

    // Trimite fișierul
    ifstream inFile(filename, ios::binary);
    char fileBuffer[1024] = {0};
    while (inFile.read(fileBuffer, 1024)) {
        send(sock, fileBuffer, inFile.gcount(), 0);
    }
    inFile.close();
    shutdown(sock, SHUT_WR);

    close(sock);
}

void requestDataFromServer() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        cout << "Socket creation error" << endl;
        return;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        cout << "Invalid address/ Address not supported" << endl;
        return;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        cout << "Connection Failed" << endl;
        return;
    }

    // Trimite cererea pentru date din baza de date
    string request = "3";
    send(sock, request.c_str(), request.size(), 0);

    // Primește răspunsul de la server
    while (true) {
        int bytesReceived = recv(sock, buffer, 1024, 0);
        if (bytesReceived <= 0)
            break;
        cout << buffer;
    }

    close(sock);
}

int main() {
    int option;
    string filename;
    while (true) {
        cout << "1. Convert DOCX to PDF" << endl;
        cout << "2. Convert PDF to DOCX" << endl;
        cout << "3. Request data from server" << endl;
        cout << "4. Exit" << endl;
        cout << "Enter option: ";
        cin >> option;

        if (option == 1 || option == 2) {
            cout << "Enter filename: ";
            cin >> filename;
            sendFile(filename, option);
        } else if (option == 3) {
            requestDataFromServer();
        } else if (option == 4) {
            cout << "Exiting..." << endl;
            break;
        } else {
            cout << "Invalid option!" << endl;
        }
    }
    return 0;
}
