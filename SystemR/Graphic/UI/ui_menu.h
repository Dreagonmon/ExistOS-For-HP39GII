#pragma once
#include <stdint.h>
#include "u8str.h"

/* A normal list menu */
int16_t ui_menu_select1(U8String title, U8StringGroup items, int16_t init_index);
/* A normal list menu, with items align center */
int16_t ui_menu_select2(U8String title, U8StringGroup items, int16_t init_index);
/* A 2-columns menu */
int16_t ui_menu_select3(U8String title, U8StringGroup items, int16_t init_index);
