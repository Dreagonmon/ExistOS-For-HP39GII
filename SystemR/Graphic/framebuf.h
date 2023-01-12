#pragma once

#include <stdint.h>

#define COLOR_BLANK 0xFF
#define COLOR_FULL 0x00

typedef struct GrayFrameBuffer GrayFrameBuffer;
typedef void (*FunctionSetPixelUnsafe)(GrayFrameBuffer *frame, uint16_t x, uint16_t y, uint8_t color);
typedef uint8_t (*FunctionGetPixelUnsafe)(GrayFrameBuffer *frame, uint16_t x, uint16_t y);
typedef struct GrayFrameBuffer {
    uint16_t width;
    uint16_t height;
    uint8_t *buffer;
    FunctionSetPixelUnsafe set_pixel_unsafe;
    FunctionGetPixelUnsafe get_pixel_unsafe;
} GrayFrameBuffer;

void gfb_clear(GrayFrameBuffer *frame, uint8_t color);
uint8_t gfb_get_pixel(GrayFrameBuffer *frame, int16_t x, int16_t y);
void gfb_set_pixel(GrayFrameBuffer *frame, int16_t x, int16_t y, uint8_t color);
void gfb_fill_rect(GrayFrameBuffer *frame, int16_t x, int16_t y, int16_t w, int16_t h, uint8_t color);
void gfb_draw_hline(GrayFrameBuffer *frame, int16_t x, int16_t y, int16_t len, uint8_t color);
void gfb_draw_vline(GrayFrameBuffer *frame, int16_t x, int16_t y, int16_t len, uint8_t color);
void gfb_draw_line(GrayFrameBuffer *frame, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t color);
/** draw 'frame_from' onto the 'frame_to'. 'ignore_color' will be seen as transparence and won't draw. */
void gfb_blit(GrayFrameBuffer *frame_to, GrayFrameBuffer *frame_from, int16_t x, int16_t y, uint8_t ignore_color);

void gfb_free(GrayFrameBuffer *frame);
GrayFrameBuffer *gfb_new_gray_frame(uint16_t w, uint16_t h);
/** a mono color frame. COLOR_BLANK as empty, other as color which is set during the creating process. */
GrayFrameBuffer *gfb_new_mono_frame(uint16_t w, uint16_t h, uint8_t color);
