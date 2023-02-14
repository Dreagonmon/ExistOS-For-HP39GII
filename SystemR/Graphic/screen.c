/**
 * hp-39gii screen virtual framebuffer
*/
#include <stdio.h>
#include "framebuf.h"
#include "sys_llapi.h"
#include "screen.h"

#define LCD_REFRESH_BUFFER_SIZE  256 // must greater than 256
#define LCD_REFRESH_BUFFER_LINES LCD_REFRESH_BUFFER_SIZE / SCR_W

static gfb_FrameBuffer *screen_frame = NULL;
static uint8_t refrsh_buffer[LCD_REFRESH_BUFFER_SIZE];
static uint8_t indicator = 0;

void screen_deinit() {
    if (screen_frame != NULL) {
        gfb_free(screen_frame);
        screen_frame = NULL;
    }
}

void screen_init_mono() {
    screen_deinit();
    screen_frame = gfb_new_mono_frame(SCR_W, SCR_H, COLOR_SET);
}

void screen_init_gray() {
    screen_deinit();
    screen_frame = gfb_new_gray_frame(SCR_W, SCR_H);
}

uint8_t check_indicator(uint8_t indicator_bit) {
    return (indicator & indicator_bit) > 0;
}

void set_indicator(uint8_t indicator_bit, uint8_t set_or_clear) {
    if (set_or_clear) {
        indicator |= indicator_bit;
    } else {
        indicator &= ~(indicator_bit);
    }
    ll_disp_set_indicator(indicator, -1);
}

gfb_FrameBuffer *get_frame_buffer() {
    return screen_frame;
}

void screen_flush() {
    if (screen_frame == NULL) {
        return;
    }
    gfb_FunctionGetPixelUnsafe get_pixel = screen_frame->get_pixel_unsafe;
    uint16_t y = 0;
    // generate and refresh buffer
    while (y < SCR_H) {
        uint16_t base_y = y;
        uint16_t iy, ix;
        for (iy = 0; iy < LCD_REFRESH_BUFFER_LINES; iy ++) {
            for (ix = 0; ix < SCR_W; ix ++) {
                refrsh_buffer[iy * SCR_W + ix] = get_pixel(screen_frame, ix, y);
            }
            y += 1;
            if (y >= SCR_H) {
                break;
            }
        }
        ll_disp_put_area(refrsh_buffer, 0, base_y, SCR_W - 1, y - 1);
    }
}
