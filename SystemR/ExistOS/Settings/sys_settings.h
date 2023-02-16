#pragma once

#include <stdint.h>

struct sys_settings {
    uint16_t settings_version;
    uint64_t rtc_offset;
    uint8_t ui_lang;
    uint8_t cpu_slowdown_mode;
    uint8_t flag1;
    uint8_t flag2;
    uint8_t settings_inited;
};

extern struct sys_settings *sys_settings;

#define sys_FLAG1_ENABLE_CHARGE_AT_BOOT           0b00000001

uint8_t init_settings(void);
void init_default_settings(void);
uint8_t save_settings(void);