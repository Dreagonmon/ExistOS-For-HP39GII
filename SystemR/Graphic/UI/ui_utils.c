#include "ui_utils.h"
#include "bmfont.h"
#include "framebuf.h"
#include "ui_const.h"
#include "u8str.h"
#include "keyboard.h"

#define MAX_LINES 32

const static uint16_t NUM_KEY_CODE[] = {
    kbd_K_0,
    kbd_K_1,
    kbd_K_2,
    kbd_K_3,
    kbd_K_4,
    kbd_K_5,
    kbd_K_6,
    kbd_K_7,
    kbd_K_8,
    kbd_K_9
};

static uint8_t ui_lang = ui_LANG_ENG;

void ui_text_area(bmf_BitmapFont *font, U8String text, gfb_FrameBuffer *frame, int16_t x, int16_t y, uint16_t w, uint16_t h, uint8_t align, uint16_t color, uint16_t bg_color) {
    // calc line width
    U8Size len = u8_string_size(text);
    U8Size fitlen = 0;
    U8Size p_off = 0;
    U8Size lws[MAX_LINES]; // line text bytes size
    uint16_t gws[MAX_LINES]; // line graphic width
    uint16_t max_width = 0;
    int16_t off_x = x;
    int16_t off_y = y;
    uint8_t lines = 0;
    uint8_t cur_lines;
    while ((p_off < len) && ((lines + 1) * font->char_height <= h) && (lines < MAX_LINES)) {
        fitlen = bmf_get_text_offset(font, text + p_off, len - p_off, w, font->char_height);
        if (fitlen == 0) {
            break;
        }
        lws[lines] = fitlen;
        gws[lines] = bmf_get_text_width(font, text + p_off, fitlen);
        max_width = (gws[lines] > max_width) ? gws[lines] : max_width;
        lines ++;
        p_off += fitlen;
    }
    // draw lines
    uint8_t xbit = w % 2;
    uint8_t ybit = h % 2;
    gfb_fill_rect(frame, x, y, w, h, bg_color);
    if (align & ui_ALIGN_VBOTTOM) {
        off_y = y + (h - (lines * font->char_height));
    }else if (align & ui_ALIGN_VCENTER) {
        off_y = y + ((h - (lines * font->char_height)) / 2) + ybit;
    }else {
        off_y = y;
    }
    p_off = 0;
    for (cur_lines = 0; cur_lines < lines; cur_lines ++) {
        if (align & ui_ALIGN_HRIGHT) {
            off_x = x + (w - gws[cur_lines]);
        } else if (align & ui_ALIGN_HCENTER) {
            off_x = x + ((w - gws[cur_lines]) / 2) + xbit;
        } else {
            off_x = x;
        }
        bmf_draw_text(font, text + p_off, lws[cur_lines], frame, off_x, off_y, gws[cur_lines], font->char_height, color);
        off_y += font->char_height;
        p_off += lws[cur_lines];
    }
}

uint8_t ui_get_key_number(uint16_t key_code) {
    uint8_t i = 0;
    while (i < 10) {
        if (key_code == NUM_KEY_CODE[i]) {
            return i;
        }
        i ++;
    }
    return ui_NOT_A_NUM_KEY;
}

void ui_set_lang(uint8_t lang) {
    ui_lang = lang;
}

uint8_t ui_get_lang() {
    return ui_lang;
}

U8String ui_trs(U8StringGroup items) {
    U8String item = u8_string_group_get(items, ui_lang);
    if (item != NULL) {
        return item;
    }
    return ui_TEXT_EMPTY;
}

U8StringGroup ui_trsg(U8StringGroupList groups) {
    U8StringGroup group = u8_string_group_list_get(groups, ui_lang);
    if (group != NULL) {
        return group;
    }
    return ui_TEXT_EMPTY;
}
