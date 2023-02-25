#include <stdint.h>
#include "u8str.h"

#define ui_RETURN_VALUE_CANCELED INT32_MIN
#define ui_NO_INIT_VALUE INT32_MIN

int32_t ui_input_number(U8String title, U8String message, int32_t min, int32_t max, int32_t init_value);
