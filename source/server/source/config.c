#include <stdlib.h>

#include "config.h"

#include <netinet/in.h>

static struct Config gConfig;

void init_config(int argc, char **argv) {
  gConfig.Ip = INADDR_ANY;
  gConfig.Port = 11001;
}

const struct Config *get_config() { return &gConfig; }
