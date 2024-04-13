#include <stdio.h>

#include "config.h"
#include "server.h"

int main(int argc, char **argv) {
  init_config(argc, argv);
  init_server();

  puts("server");
  return 0;
}