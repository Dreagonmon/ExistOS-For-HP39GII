/**
 * https://github.com/esmil/musl
*/
#include <stdint.h>

int32_t uclock_month_to_secs(int32_t month, int32_t is_leap)
{
	static const int32_t secs_through_month[] = {
		0, 31*86400, 59*86400, 90*86400,
		120*86400, 151*86400, 181*86400, 212*86400,
		243*86400, 273*86400, 304*86400, 334*86400 };
	int32_t t = secs_through_month[month];
	if (is_leap && month >= 2) t+=86400;
	return t;
}
