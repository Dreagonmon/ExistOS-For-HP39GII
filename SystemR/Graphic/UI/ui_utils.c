#include "ui_utils.h"
#include "bmfont.h"
#include "framebuf.h"

#define MAX_LINES 32

static uint8_t ui_lang = ui_LANG_ENG;

void ui_text_area(bmf_BitmapFont *font, const StrItem text, gfb_FrameBuffer *frame, int16_t x, int16_t y, uint16_t w, uint16_t h, uint8_t align, uint16_t color, uint16_t bg_color) {
    // calc line width
    uint32_t lws[MAX_LINES]; // line text bytes size
    uint16_t gws[MAX_LINES]; // line graphic width
    uint16_t max_width = 0;
    uint32_t p_off = 0;
    uint32_t fitlen = 0;
    int16_t off_x = x;
    int16_t off_y = y;
    uint8_t lines = 0;
    uint8_t cur_lines;
    while ((p_off < text.len) && ((lines + 1) * font->char_height <= h) && (lines < MAX_LINES)) {
        fitlen = bmf_get_text_offset(font, text.text + p_off, text.len - p_off, w, font->char_height);
        if (fitlen == 0) {
            break;
        }
        lws[lines] = fitlen;
        gws[lines] = bmf_get_text_width(font, text.text + p_off, fitlen);
        max_width = (gws[lines] > max_width) ? gws[lines] : max_width;
        lines ++;
        p_off += fitlen;
    }
    // draw lines
    gfb_fill_rect(frame, x, y, w, h, bg_color);
    if (align & ui_ALIGN_VBOTTOM) {
        off_y = y + (h - (lines * font->char_height));
    }else if (align & ui_ALIGN_VCENTER) {
        off_y = y + ((h - (lines * font->char_height)) / 2);
    }else {
        off_y = y;
    }
    p_off = 0;
    for (cur_lines = 0; cur_lines < lines; cur_lines ++) {
        if (align & ui_ALIGN_HRIGHT) {
            off_x = x + (w - gws[cur_lines]);
        } else if (align & ui_ALIGN_HCENTER) {
            off_x = x + ((w - gws[cur_lines]) / 2);
        } else {
            off_x = x;
        }
        bmf_draw_text(font, text.text + p_off, lws[cur_lines], frame, off_x, off_y, gws[cur_lines], font->char_height, color);
        off_y += font->char_height;
        p_off += lws[cur_lines];
    }
}


uint16_t ui_get_stritems_count(const StrItem *items) {
    uint16_t len = 0;
    while (items[len].len > 0) {
        len ++;
    }
    return len;
}

void ui_set_lang(uint8_t lang) {
    ui_lang = lang;
}

const StrItem ui_trs(const StrItem *items) {
    uint16_t len = ui_get_stritems_count(items);
    if (ui_lang >= len) {
        return stritem("");
    } else {
        return items[ui_lang];
    }
}
