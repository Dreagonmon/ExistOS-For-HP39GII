/**
 * https://github.com/esmil/musl
*/
#include <stdint.h>
#include "uclock.h"

int64_t uclock_tm_to_secs(const struct utm *tm)
{
	int32_t is_leap;
	int64_t year = tm->tm_year;
	int32_t month = tm->tm_mon;
	if (month >= 12 || month < 0) {
		int32_t adj = month / 12;
		month %= 12;
		if (month < 0) {
			adj--;
			month += 12;
		}
		year += adj;
	}
	int64_t t = uclock_year_to_secs(year, &is_leap);
	t += uclock_month_to_secs(month, is_leap);
	t += 86400LL * (tm->tm_mday-1);
	t += 3600LL * tm->tm_hour;
	t += 60LL * tm->tm_min;
	t += tm->tm_sec;
	return t;
}
