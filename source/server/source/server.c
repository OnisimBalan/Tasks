#include "server.h"
#include "config.h"

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>

static int gServerSocket = 0;

static void init_server_socket() {
  // Create socket
  gServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (gServerSocket == -1) {
    puts("Failed to create server socket!");
    exit(-1);
  }

  const struct Config *config = get_config();

  // Bind socket
  struct sockaddr_in serv_addr;
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = config->Ip;
  serv_addr.sin_port = htons(config->Port);

  int result = bind(gServerSocket, (const struct sockaddr *)&serv_addr,
                    sizeof(serv_addr));
  if (result < 0) {
    puts("Failed to bind server socket!");
    exit(-1);
  }

  result = listen(gServerSocket, 1000);
  if (result < 0) {
    puts("Failed to listen on server socket!");
    exit(-1);
  }
}

void init_server() {
  init_server_socket();

    // this will be on the cline side (test)
    //start
  struct sockaddr_in cli_addr;
  socklen_t cli_addr_len = sizeof(cli_addr);
  int result =
      accept(gServerSocket, (struct sockaddr *)&cli_addr, &cli_addr_len);
  if (result >= 0) {
    puts("accepted client!");
  } else {
    puts("falied accepted client!");
  }
    //end
}

void run_server() {}
