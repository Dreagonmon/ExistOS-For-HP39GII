#pragma once

#include <stdint.h>
#include "u8str.h"

void ui_sysbar_title(U8String title);
void ui_sysbar_fn_set_cell(uint8_t n, U8String title);
void ui_sysbar_fn_text(uint8_t start_n, uint8_t size_n, U8String text);
void ui_sysbar_fn_clear();
