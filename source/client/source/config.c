#include <stdlib.h>

#include "config.h"

static struct Config gConfig;

void init_config(int argc, char**argv)
{
   
}

const struct Config* get_config()
{
    return &gConfig;
}
