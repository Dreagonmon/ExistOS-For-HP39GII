#pragma once

#include <stdint.h>
#include <stddef.h>
#include "bmfont.h"
#include "framebuf.h"
#include "u8str.h"

#define ui_ALIGN_HLEFT                0b00000001
#define ui_ALIGN_HCENTER              0b00000010
#define ui_ALIGN_HRIGHT               0b00000100
#define ui_ALIGN_VTOP                 0b00010000
#define ui_ALIGN_VCENTER              0b00100000
#define ui_ALIGN_VBOTTOM              0b01000000
#define ui_NOT_A_NUM_KEY                 UINT8_MAX
#define ui_LANG_ENG                   0x00
#define ui_LANG_CHS                   0x01

void ui_text_area(bmf_BitmapFont *font, U8String text, gfb_FrameBuffer *frame, int16_t x, int16_t y, uint16_t w, uint16_t h, uint8_t align, uint16_t color, uint16_t bg_color);
uint8_t ui_get_key_number(uint16_t key_code);
void ui_set_lang(uint8_t lang);
uint8_t ui_get_lang();
U8String ui_trs(U8StringGroup items);
U8StringGroup ui_trsg(U8StringGroupList groups);
