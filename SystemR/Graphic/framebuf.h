#pragma once

#include <stdint.h>

#define COLOR_BLANK 0xFF
#define COLOR_FULL 0x00

typedef struct gfb_GrayFrameBuffer gfb_GrayFrameBuffer;
typedef void (*gfb_FunctionSetPixelUnsafe)(gfb_GrayFrameBuffer *frame, uint16_t x, uint16_t y, uint8_t color);
typedef uint8_t (*gfb_FunctionGetPixelUnsafe)(gfb_GrayFrameBuffer *frame, uint16_t x, uint16_t y);
typedef struct gfb_GrayFrameBuffer {
    uint16_t width;
    uint16_t height;
    uint8_t *buffer;
    gfb_FunctionSetPixelUnsafe set_pixel_unsafe;
    gfb_FunctionGetPixelUnsafe get_pixel_unsafe;
} gfb_GrayFrameBuffer;

void gfb_clear(gfb_GrayFrameBuffer *frame, uint8_t color);
uint8_t gfb_get_pixel(gfb_GrayFrameBuffer *frame, int16_t x, int16_t y);
void gfb_set_pixel(gfb_GrayFrameBuffer *frame, int16_t x, int16_t y, uint8_t color);
void gfb_fill_rect(gfb_GrayFrameBuffer *frame, int16_t x, int16_t y, int16_t w, int16_t h, uint8_t color);
void gfb_draw_hline(gfb_GrayFrameBuffer *frame, int16_t x, int16_t y, int16_t len, uint8_t color);
void gfb_draw_vline(gfb_GrayFrameBuffer *frame, int16_t x, int16_t y, int16_t len, uint8_t color);
void gfb_draw_line(gfb_GrayFrameBuffer *frame, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t color);
/** draw 'frame_from' onto the 'frame_to'. 'ignore_color' will be seen as transparence and won't draw. */
void gfb_blit(gfb_GrayFrameBuffer *frame_to, gfb_GrayFrameBuffer *frame_from, int16_t x, int16_t y, uint8_t ignore_color);

void gfb_free(gfb_GrayFrameBuffer *frame);
gfb_GrayFrameBuffer *gfb_new_gray_frame(uint16_t w, uint16_t h);
/** a mono color frame. COLOR_BLANK as empty, other as color which is set during the creating process. */
gfb_GrayFrameBuffer *gfb_new_mono_frame(uint16_t w, uint16_t h, uint8_t color);
