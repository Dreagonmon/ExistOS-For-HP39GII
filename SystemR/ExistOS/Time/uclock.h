#pragma once

#include <stdint.h>

struct utm {
    int32_t tm_sec;
    int32_t tm_min;
    int32_t tm_hour;
    int32_t tm_mday;
    int32_t tm_mon;
    int32_t tm_year;
    int32_t tm_wday;
    int32_t tm_yday;
    int32_t tm_isdst;
};

int8_t uclock_secs_to_tm(int64_t t, struct utm *tm);
int32_t uclock_month_to_secs(int32_t month, int32_t is_leap);
int64_t uclock_year_to_secs(int64_t year, int32_t *is_leap);
int64_t uclock_tm_to_secs(const struct utm *tm);
struct utm uclock_gmtime(const int64_t *sec);
int64_t mktime(const struct utm *tm);
