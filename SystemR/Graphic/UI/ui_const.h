#pragma once
#include "screen.h"
#include "u8str.h"

#define ui_TITLEBAR_X 0
#define ui_TITLEBAR_Y 0
#define ui_TITLEBAR_H 17
#define ui_TITLEBAR_W SCR_W
#define ui_FNBAR_W 41
#define ui_FNBAR_H 14
#define ui_FNBAR_GAP 2
#define ui_FNBAR_AREA_X 0
#define ui_FNBAR_AREA_Y (SCR_H - ui_FNBAR_H)
#define ui_FNBAR_AREA_W SCR_W
#define ui_FNBAR_AREA_H ui_FNBAR_H
#define ui_FNBAR_X(n) ((ui_FNBAR_W + ui_FNBAR_GAP) * n)
#define ui_FNBAR_Y ui_FNBAR_AREA_Y
#define ui_CONTENT_X 0
#define ui_CONTENT_Y ui_TITLEBAR_H
#define ui_CONTENT_W SCR_W
#define ui_CONTENT_H (SCR_H - ui_FNBAR_H - ui_TITLEBAR_H)

extern U8String ui_TEXT_EMPTY;
extern U8StringGroup ui_TEXTG_OK;
extern U8StringGroup ui_TEXTG_CONFIRM;
extern U8StringGroup ui_TEXTG_CANCEL;
extern U8StringGroup ui_TEXTG_PAGE_UP;
extern U8StringGroup ui_TEXTG_PAGE_DOWN;
extern U8StringGroup ui_TEXTG_INFO;
extern U8StringGroup ui_TEXTG_WARNING;
extern U8StringGroup ui_TEXTG_ERROR;
