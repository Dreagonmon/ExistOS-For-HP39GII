#pragma once

#include <stdint.h>
#include "u8str.h"

uint8_t ui_dialog_confirm(U8String title, U8String content);
void ui_dialog_alert(U8String title, U8String content);
