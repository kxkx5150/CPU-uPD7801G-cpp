#ifndef _CONFIG_H_
#define _CONFIG_H_

#include "vm.h"
#include "windef.h"

#define CONFIG_VERSION 0x13

void init_config();
void load_config();
void save_config();

typedef struct
{
    int  version;
    int  cpu_power;
    int  sound_frequency;
    int  sound_latency;
    char recent_cart[8][MAX_PATH];


} config_t;

#endif
