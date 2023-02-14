#pragma once

#include <stdint.h>

#define COLOR_CLEAR 0xFF
#define COLOR_SET 0x00

typedef struct gfb_FrameBuffer gfb_FrameBuffer;
typedef void (*gfb_FunctionSetPixelUnsafe)(gfb_FrameBuffer *frame, uint16_t x, uint16_t y, uint16_t color);
typedef uint16_t (*gfb_FunctionGetPixelUnsafe)(gfb_FrameBuffer *frame, uint16_t x, uint16_t y);
typedef struct gfb_FrameBuffer {
    uint16_t width;
    uint16_t height;
    gfb_FunctionSetPixelUnsafe set_pixel_unsafe;
    gfb_FunctionGetPixelUnsafe get_pixel_unsafe;
    uint8_t *buffer;
} gfb_FrameBuffer;

void gfb_clear(gfb_FrameBuffer *frame, uint16_t color);
uint16_t gfb_get_pixel(gfb_FrameBuffer *frame, int16_t x, int16_t y);
void gfb_set_pixel(gfb_FrameBuffer *frame, int16_t x, int16_t y, uint16_t color);
void gfb_fill_rect(gfb_FrameBuffer *frame, int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void gfb_draw_hline(gfb_FrameBuffer *frame, int16_t x, int16_t y, int16_t len, uint16_t color);
void gfb_draw_vline(gfb_FrameBuffer *frame, int16_t x, int16_t y, int16_t len, uint16_t color);
void gfb_draw_line(gfb_FrameBuffer *frame, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
/** draw 'frame_from' onto the 'frame_to'. 'ignore_color' will be seen as transparence and won't draw. */
void gfb_blit(gfb_FrameBuffer *frame_to, gfb_FrameBuffer *frame_from, int16_t x, int16_t y, uint16_t ignore_color);

void gfb_free(gfb_FrameBuffer *frame);
gfb_FrameBuffer *gfb_new_gray_frame(uint16_t w, uint16_t h);
/** a mono color frame. COLOR_CLEAR as empty, other as color which is set during the creating process. */
gfb_FrameBuffer *gfb_new_mono_frame(uint16_t w, uint16_t h, uint16_t color);
