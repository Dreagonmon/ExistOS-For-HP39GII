#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "FreeRTOS.h"
#include "basic_api.h"
#include "sys_llapi.h"
#include "framebuf.h"
#include "screen.h"

GrayFrameBuffer *frame = NULL;
uint8_t background_color = 0x1F;

void display() {
    // api_vram_flush();
    printf("screen_frame addr %d\n", screen_frame);
    gfb_clear(screen_frame, background_color);
    gfb_blit(screen_frame, frame, 64, 32, COLOR_BLANK);
    screen_flush();
}

/* System Init */
void main_init() {
    ll_cpu_slowdown_enable(false);
    screen_init();
    frame = gfb_new_mono_frame(LCD_PIX_W, LCD_PIX_H, COLOR_FULL);
    // frame = gfb_new_gray_frame(LCD_PIX_W, LCD_PIX_H);
    gfb_clear(frame, COLOR_BLANK);
    gfb_set_pixel(frame, 64, 64, COLOR_FULL);
    gfb_draw_hline(frame, -5, 66, 512, COLOR_FULL);
    gfb_draw_vline(frame, 66, -2, 256, COLOR_FULL);
    gfb_draw_line(frame, 60, 66, 66, 60, COLOR_FULL);
}

void main() {
    main_init();
    printf("Hello Dragon\n");
    printf("================================================\n");
    while (1) {
        int key = api_get_key(-1);
        printf("key: %d\n", key);
        display();
        background_color -= 0x20;
        // background_color = background_color % 256;
    }
}