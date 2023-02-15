#include "ui_sysbar.h"
#include "ui_utils.h"
#include "ui_const.h"
#include "screen.h"
#include "framebuf.h"
#include "font8x8.h"
#include "font16x16.h"

void ui_sysbar_title(U8String title) {
    ui_text_area(
        font16x16_unifont, title, get_frame_buffer(),
        ui_TITLEBAR_X, ui_TITLEBAR_Y, ui_TITLEBAR_W, ui_TITLEBAR_H - 1,
        ui_ALIGN_HCENTER | ui_ALIGN_VCENTER,
        COLOR_CLEAR, COLOR_SET
    );
    gfb_fill_rect(
        get_frame_buffer(),
        ui_TITLEBAR_X, ui_TITLEBAR_Y + ui_TITLEBAR_H - 1, ui_TITLEBAR_W, 1,
        COLOR_SET
    );
}

void ui_sysbar_fn_set_cell(uint8_t n, U8String title) {
    n %= 6;
    ui_text_area(
        font8x8_quan, title, get_frame_buffer(),
        ui_FNBAR_X(n), ui_FNBAR_Y, ui_FNBAR_W, ui_FNBAR_H,
        ui_ALIGN_HCENTER | ui_ALIGN_VCENTER,
        COLOR_CLEAR, COLOR_SET
    );
}

void ui_sysbar_fn_text(uint8_t start_n, uint8_t size_n, U8String text) {
    start_n = (start_n > 6) ? 6 : start_n;
    size_n = (size_n + start_n > 6) ? 6 - start_n : size_n;
    if (size_n <= 0) {
        return;
    }
    uint16_t area_x = ui_FNBAR_X(start_n);
    uint16_t area_w = (ui_FNBAR_W * size_n) + (ui_FNBAR_GAP * (size_n - 1));
    ui_text_area(
        font8x8_quan, text, get_frame_buffer(),
        area_x, ui_FNBAR_Y, area_w, ui_FNBAR_H,
        ui_ALIGN_HCENTER | ui_ALIGN_VCENTER,
        COLOR_CLEAR, COLOR_SET
    );
}

void ui_sysbar_fn_clear() {
    gfb_fill_rect(
        get_frame_buffer(),
        ui_FNBAR_AREA_X, ui_FNBAR_AREA_Y, ui_FNBAR_AREA_W, ui_FNBAR_AREA_H,
        COLOR_CLEAR
    );
}
