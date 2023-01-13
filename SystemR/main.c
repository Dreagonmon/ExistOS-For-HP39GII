#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "FreeRTOS.h"
#include "basic_api.h"
#include "sys_llapi.h"
#include "framebuf.h"
#include "screen.h"

GrayFrameBuffer *frame = NULL;
int16_t offx = 0;
int16_t offy = 0;

void display() {
    gfb_clear(screen_frame, COLOR_BLANK);
    gfb_blit(screen_frame, frame, offx, offy, COLOR_BLANK);
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
    display();
    int KEY_UP = (0 << 3) + 4;
    int KEY_RIGHT = (1 << 3) + 4;
    int KEY_LEFT = (2 << 3) + 4;
    int KEY_DOWN = (3 << 3) + 4;
    while (1) {
        if (api_get_key(KEY_UP)) {
            // printf("key: %d\n", key);
            offy -= 2;
            display();
        }
        if (api_get_key(KEY_DOWN)) {
            // printf("key: %d\n", key);
            offy += 2;
            display();
        }
        if (api_get_key(KEY_LEFT)) {
            // printf("key: %d\n", key);
            offx -= 2;
            display();
        }
        if (api_get_key(KEY_RIGHT)) {
            // printf("key: %d\n", key);
            offx += 2;
            display();
        }
    }
}