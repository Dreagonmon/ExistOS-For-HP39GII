#pragma once

#include <stdint.h>
#include "framebuf.h"

typedef struct bmf_BitmapFontFile bmf_BitmapFontFile;
typedef struct bmf_BitmapFont bmf_BitmapFont;
typedef void (*bmf_FunctionSeek)(bmf_BitmapFontFile *ffile, uint32_t offset);
typedef uint32_t (*bmf_FunctionRead)(bmf_BitmapFontFile *ffile, uint8_t *buffer, uint32_t size);
typedef uint8_t (*bmf_FunctionGetCharImage)(bmf_BitmapFont *font, uint32_t unicode, uint8_t *buffer); // MONO_HMSB format
typedef struct bmf_BitmapFontFile {
    bmf_FunctionSeek seek;
    bmf_FunctionRead read;
    void *data;
    void *file_ptr;
} bmf_BitmapFontFile;
typedef struct bmf_BitmapFont {
    bmf_FunctionGetCharImage get_char_image;
    bmf_BitmapFontFile *font_file;
    const uint8_t char_width;
    const uint8_t char_height;
    const uint8_t *ascii_width; // size [94]
} bmf_BitmapFont;

uint16_t bmf_get_text_width(bmf_BitmapFont *font, const char *text, uint32_t bytes_len);
uint32_t bmf_draw_text(bmf_BitmapFont *font, const char *text, uint32_t bytes_len, gfb_FrameBuffer *frame, int16_t x, int16_t y, uint16_t width_limit, uint16_t height_limit, uint16_t color);
uint32_t bmf_get_text_offset(bmf_BitmapFont *font, const char *text, uint32_t bytes_len, uint16_t width_limit, uint16_t height_limit);
