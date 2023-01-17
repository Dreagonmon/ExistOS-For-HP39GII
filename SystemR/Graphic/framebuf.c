#include <stdio.h>
#include "framebuf.h"
#include "FreeRTOS.h"

#define max(a,b) ((a) >= (b) ? (a) : (b))
#define min(a,b) ((a) <= (b) ? (a) : (b))
#define abs(x) (((int32_t)x) < 0 ? (-x) : (x))

#define malloc(s) pvPortMalloc(s)
#define free(p) vPortFree(p)

void gfb_clear(gfb_GrayFrameBuffer *frame, uint8_t color) {
    gfb_FunctionSetPixelUnsafe set_pixel = frame->set_pixel_unsafe;
    uint16_t width = frame->width;
    uint16_t height = frame->height;
    uint16_t x, y;
    for (y = 0; y < height; y++) {
        for (x = 0; x < width; x++) {
            //
            set_pixel(frame, x, y, color);
        }
    }
}

uint8_t gfb_get_pixel(gfb_GrayFrameBuffer *frame, int16_t x, int16_t y) {
    if((x >= 0) && (y >= 0) && (x < frame->width) && (y < frame->height)) {
		return (*(frame->get_pixel_unsafe))(frame, x, y);
	} else {
        return COLOR_BLANK;
    }
}

void gfb_set_pixel(gfb_GrayFrameBuffer *frame, int16_t x, int16_t y, uint8_t color) {
    if((x >= 0) && (y >= 0) && (x < frame->width) && (y < frame->height)) {
		(*(frame->set_pixel_unsafe))(frame, x, y, color);
	}
}

void gfb_fill_rect(gfb_GrayFrameBuffer *frame, int16_t x, int16_t y, int16_t w, int16_t h, uint8_t color) {
    if (h < 1 || w < 1 || x + w <= 0 || y + h <= 0 || y >= frame->height || x >= frame->width) {
        // No operation needed.
        return;
    }
    gfb_FunctionSetPixelUnsafe set_pixel = frame->set_pixel_unsafe;
    // clip to the framebuffer
    int16_t xend = min(frame->width, x + w);
    int16_t yend = min(frame->height, y + h);
    x = max(x, 0);
    y = max(y, 0);
    int16_t ix;
    for ( ; y < yend; y++) {
        for (ix = x ; ix < xend; ix++) {
            set_pixel(frame, ix, y, color);
        }
    }
}

void gfb_draw_hline(gfb_GrayFrameBuffer *frame, int16_t x, int16_t y, int16_t len, uint8_t color) {
    gfb_fill_rect(frame, x, y, len, 1, color);
}

void gfb_draw_vline(gfb_GrayFrameBuffer *frame, int16_t x, int16_t y, int16_t len, uint8_t color) {
    gfb_fill_rect(frame, x, y, 1, len, color);
}

void gfb_draw_line(gfb_GrayFrameBuffer *frame, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t color) {
    gfb_FunctionSetPixelUnsafe set_pixel = frame->set_pixel_unsafe;
    uint16_t width = frame->width;
    uint16_t height = frame->height;
    int16_t dx = x2 - x1;
    int16_t sx;
    if (dx > 0) {
        sx = 1;
    } else {
        dx = -dx;
        sx = -1;
    }
    int16_t dy = y2 - y1;
    int16_t sy;
    if (dy > 0) {
        sy = 1;
    } else {
        dy = -dy;
        sy = -1;
    }
    uint8_t steep;
    if (dy > dx) {
        int16_t temp;
        temp = x1;
        x1 = y1;
        y1 = temp;
        temp = dx;
        dx = dy;
        dy = temp;
        temp = sx;
        sx = sy;
        sy = temp;
        steep = 1;
    } else {
        steep = 0;
    }
    int16_t e = 2 * dy - dx;
    int16_t i;
    for (i = 0; i < dx; ++i) {
        if (steep) {
            if (0 <= y1 && y1 < width && 0 <= x1 && x1 < height) {
                set_pixel(frame, y1, x1, color);
            }
        } else {
            if (0 <= x1 && x1 < width && 0 <= y1 && y1 < height) {
                set_pixel(frame, x1, y1, color);
            }
        }
        while (e >= 0) {
            y1 += sy;
            e -= 2 * dx;
        }
        x1 += sx;
        e += 2 * dy;
    }
    if (0 <= x2 && x2 < width && 0 <= y2 && y2 < height) {
        set_pixel(frame, x2, y2, color);
    }
}

void gfb_blit(gfb_GrayFrameBuffer *frame_to, gfb_GrayFrameBuffer *frame_from, int16_t x, int16_t y, uint8_t ignore_color) {
    gfb_FunctionGetPixelUnsafe get_pixel_from = frame_from->get_pixel_unsafe;
    gfb_FunctionSetPixelUnsafe set_pixel_to = frame_to->set_pixel_unsafe;
    if (frame_from == NULL) {
        return;
    }
    if (
        (x >= frame_to->width) ||
        (y >= frame_to->height) ||
        (-x >= frame_from->width) ||
        (-y >= frame_from->height)
        ) {
        // Out of bounds, no-op.
        return;
    }
    // Clip.
    int16_t x0 = max(0, x);
    int16_t y0 = max(0, y);
    int16_t x1 = max(0, -x);
    int16_t y1 = max(0, -y);
    int16_t x0end = min(frame_to->width, x + frame_from->width);
    int16_t y0end = min(frame_to->height, y + frame_from->height);
    uint8_t col;
    for (; y0 < y0end; ++y0) {
        int cx1 = x1;
        for (int cx0 = x0; cx0 < x0end; ++cx0) {
            col = get_pixel_from(frame_from, cx1, y1);
            if (col != ignore_color) {
                set_pixel_to(frame_to, cx0, y0, col);
            }
            ++cx1;
        }
        ++y1;
    }
}

/* ================ */
/* in-memory frame */

void _set_gray_pixel_unsafe(gfb_GrayFrameBuffer *frame, uint16_t x, uint16_t y, uint8_t c) {
    frame->buffer[y*frame->width + x] = c;
}

uint8_t _get_gray_pixel_unsafe(gfb_GrayFrameBuffer *frame, uint16_t x, uint16_t y) {
	return frame->buffer[y*frame->width + x];
}

void _set_mono_pixel_unsafe(gfb_GrayFrameBuffer *frame, uint16_t x, uint16_t y, uint8_t c) {
    // MVLSB format
    uint8_t color = (c != COLOR_BLANK) ? 1 : 0;
    size_t index = (y >> 3) * frame->width + x + 1;
    uint8_t offset = y & 0x07;
    (frame->buffer)[index] = ((frame->buffer)[index] & ~(0x01 << offset)) | ((color != 0) << offset);
}

uint8_t _get_mono_pixel_unsafe(gfb_GrayFrameBuffer *frame, uint16_t x, uint16_t y) {
    // MVLSB format
    uint8_t color = (frame->buffer)[0];
    size_t index = (y >> 3) * frame->width + x + 1;
    uint8_t offset = y & 0x07;
    uint8_t value = ((frame->buffer)[index] >> offset) & 0x01;
    return value != 0 ? color : COLOR_BLANK;
}

void gfb_free(gfb_GrayFrameBuffer *frame) {
    if (frame == NULL) {
        return;
    }
    if (frame->buffer != NULL) {
        free(frame->buffer);
    }
    free(frame);
}

gfb_GrayFrameBuffer *gfb_new_gray_frame(uint16_t w, uint16_t h) {
    gfb_GrayFrameBuffer *frame = malloc(sizeof(gfb_GrayFrameBuffer));
    if (frame != NULL) {
        uint8_t *buffer = malloc(w * h);
        if (buffer != NULL) {
            frame->buffer = buffer;
            frame->width = w;
            frame->height = h;
            frame->set_pixel_unsafe = _set_gray_pixel_unsafe;
            frame->get_pixel_unsafe = _get_gray_pixel_unsafe;
        } else {
            free(frame);
            frame = NULL;
        }
    }
    return frame;
}

gfb_GrayFrameBuffer *gfb_new_mono_frame(uint16_t w, uint16_t h, uint8_t color) {
    gfb_GrayFrameBuffer *frame = malloc(sizeof(gfb_GrayFrameBuffer));
    if (frame != NULL) {
        uint16_t hp = h / 8;
        hp += (h % 8) ? 1 : 0;
        uint8_t *buffer = malloc((w * hp) + 1);
        if (buffer != NULL) {
            buffer[0] = color;
            frame->buffer = buffer;
            frame->width = w;
            frame->height = h;
            frame->set_pixel_unsafe = _set_mono_pixel_unsafe;
            frame->get_pixel_unsafe = _get_mono_pixel_unsafe;
        } else {
            free(frame);
            frame = NULL;
        }
    }
    return frame;
}
