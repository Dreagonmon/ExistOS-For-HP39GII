#include "sys_clock.h"
#include "sys_llapi.h"
#include "sys_settings.h"

uint64_t rtc_time() {
    if (sys_settings->settings_inited) {
        return (uint64_t)ll_rtc_get_sec() + sys_settings->rtc_offset;
    }
    return ll_rtc_get_sec();
}

int32_t ticks_s() {
    return ll_rtc_get_sec() & INT32_MAX;
}

int32_t ticks_ms() {
    return ll_get_time_ms() & INT32_MAX;
}

int32_t ticks_us() {
    return ll_get_time_us() & INT32_MAX;
}

int32_t ticks_add(int32_t t1, int32_t delta) {
    return (t1 + delta) & INT32_MAX;
}

int32_t ticks_diff(int32_t t1, int32_t t2) {
    int32_t half = (INT32_MAX / 2) + 1;
    return ((t1 - t2 + half) & INT32_MAX) - half;
}

extern bool g_system_in_emulator;
void sleep_ms(uint32_t ms) {
    if (g_system_in_emulator) {
        int32_t start_tm = ticks_ms();
        while (ticks_diff(ticks_ms(), start_tm) < ms);
    } else {
        ll_vm_sleep_ms(ms); // doesn't work in emu
    }
}
