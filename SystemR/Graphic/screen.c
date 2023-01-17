/**
 * hp-39gii screen virtual framebuffer
*/
#include <stdio.h>
#include "framebuf.h"
#include "sys_llapi.h"

#define LCD_PIX_W   256
#define LCD_PIX_H   127
#define LCD_REFRESH_BUFFER_SIZE  256 // must greater than 256
#define LCD_REFRESH_BUFFER_LINES LCD_REFRESH_BUFFER_SIZE / LCD_PIX_W

gfb_GrayFrameBuffer *screen_frame = NULL;
static uint8_t refrsh_buffer[LCD_REFRESH_BUFFER_SIZE];

void screen_deinit() {
    if (screen_frame != NULL) {
        gfb_free(screen_frame);
        screen_frame = NULL;
    }
}

void screen_init_mono() {
    screen_deinit();
    screen_frame = gfb_new_mono_frame(LCD_PIX_W, LCD_PIX_H, COLOR_FULL);
}

void screen_init_gray() {
    screen_deinit();
    screen_frame = gfb_new_gray_frame(LCD_PIX_W, LCD_PIX_H);
}

void screen_flush() {
    if (screen_frame == NULL) {
        return;
    }
    gfb_FunctionGetPixelUnsafe get_pixel = screen_frame->get_pixel_unsafe;
    uint16_t y = 0;
    // generate and refresh buffer
    while (y < LCD_PIX_H) {
        uint16_t base_y = y;
        uint16_t iy, ix;
        for (iy = 0; iy < LCD_REFRESH_BUFFER_LINES; iy ++) {
            for (ix = 0; ix < LCD_PIX_W; ix ++) {
                refrsh_buffer[iy * LCD_PIX_W + ix] = get_pixel(screen_frame, ix, y);
            }
            y += 1;
            if (y >= LCD_PIX_H) {
                break;
            }
        }
        ll_disp_put_area(refrsh_buffer, 0, base_y, LCD_PIX_W - 1, y - 1);
    }
}
