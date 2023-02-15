#pragma once

#include <stdint.h>
#include <stddef.h>
#include "bmfont.h"
#include "framebuf.h"

#define ui_ALIGN_HLEFT                0b00000001
#define ui_ALIGN_HCENTER              0b00000010
#define ui_ALIGN_HRIGHT               0b00000100
#define ui_ALIGN_VTOP                 0b00010000
#define ui_ALIGN_VCENTER              0b00100000
#define ui_ALIGN_VBOTTOM              0b01000000
#define ui_LANG_ENG                   0x00
#define ui_LANG_CHS                   0x01

typedef struct
{
    const uint16_t len;
    const char *text;
} StrItem;
/* const StrItem */
typedef const StrItem CStrItem;
/* StrItemGroup is a StrItem list that ends with NULL_STR_ITEM */
typedef StrItem* StrItemGroup;
/* CStrItemGroup is a const StrItem list that ends with NULL_STR_ITEM */
typedef CStrItem* CStrItemGroup;
/* CCStrItemGroup is a const StrItem list that ends with NULL_STR_ITEM, and it self is const. */
typedef const CStrItemGroup CCStrItemGroup;

#define stritem(x) ((CStrItem){.len = (const uint16_t)(sizeof(x)), .text = (const char *)(x)})
#define NULL_STR_ITEM ((CStrItem){.len = 0, .text = NULL})
#define stritemgroup(...) { __VA_ARGS__, NULL_STR_ITEM }

extern CCStrItemGroup TEXTG_CONFIRM;
extern CCStrItemGroup TEXTG_CANCEL;
extern CCStrItemGroup TEXTG_PAGE_UP;
extern CCStrItemGroup TEXTG_PAGE_DOWN;
extern CStrItem TEXT_EMPTY;

void ui_text_area(bmf_BitmapFont *font, const StrItem text, gfb_FrameBuffer *frame, int16_t x, int16_t y, uint16_t w, uint16_t h, uint8_t align, uint16_t color, uint16_t bg_color);
void ui_set_lang(uint8_t lang);
uint8_t ui_get_lang();
uint16_t ui_get_stritems_count(CCStrItemGroup items);
CStrItem ui_trs(CCStrItemGroup items);
CStrItem *ui_trsp(CCStrItemGroup items);
/* translate CCStrItemGroup list, list end with NULL */
CCStrItemGroup ui_trsg(const CCStrItemGroup *groups);
