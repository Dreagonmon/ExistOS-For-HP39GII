#include <stddef.h>
#include "uclock.h"
#include "sys_clock.h"

struct utm _global_utm_struct = {0};

struct utm uclock_gmtime(const int64_t *sec) {
    struct utm tm = {0};
    int64_t seconds = 0;
    if (sec == NULL) {
        seconds = rtc_time() & INT64_MAX;
    } else {
        seconds = *sec;
    }
    uclock_secs_to_tm(seconds, &tm);
    // TODO: check year overflow int32
    return tm;
}

int64_t mktime(const struct utm *tm) {
    return uclock_tm_to_secs(tm);
}
