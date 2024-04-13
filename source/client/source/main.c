#include <stdio.h>

#include "config.h"

int main(int argc, char **argv) {
  init_config(argc, argv);

  puts("client");
  return 0;
}
