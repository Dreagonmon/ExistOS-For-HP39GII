#include "sys_clock.h"
#include "sys_llapi.h"
#include "time.h"

static struct tm global_tm = { 0 };

uint64_t rtc_time() {
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
