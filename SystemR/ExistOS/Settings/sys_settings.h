#pragma once

#include <stdint.h>

struct sys_settings {
    uint16_t settings_version;
    uint64_t rtc_offset;
    uint8_t settings_inited;
};

extern struct sys_settings *sys_settings;

uint8_t init_settings(void);
void init_default_settings(void);
uint8_t save_settings(void);