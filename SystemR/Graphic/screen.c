/**
 * hp-39gii screen virtual framebuffer
*/
#include <stdio.h>
#include "framebuf.h"
#include "sys_llapi.h"

#define LCD_PIX_W   256
#define LCD_PIX_H   127

GrayFrameBuffer *screen_frame = NULL;

void screen_init() {
    screen_frame = gfb_new_gray_frame(LCD_PIX_W, LCD_PIX_H);
}

void screen_flush() {
    ll_disp_put_area(screen_frame->buffer, 0, 0, LCD_PIX_W - 1, LCD_PIX_H - 1);
}
