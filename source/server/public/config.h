#ifndef TASKS_CONFIG_H
#define TASKS_CONFIG_H

#include <stdint.h>

struct Config
{
    uint32_t Ip;
    uint16_t Port;
};

extern void init_config(int argc, char**argv);
extern const struct Config* get_config();

#endif