#include "ui_sysbar.h"
#include "ui_utils.h"
#include "screen.h"
#include "framebuf.h"
#include "font8x8.h"
#include "font16x16.h"

void ui_sysbar_title(const StrItem title) {
    ui_text_area(
        font16x16_unifont, title, get_frame_buffer(),
        ui_TITLEBAR_X, ui_TITLEBAR_Y, ui_TITLEBAR_W, ui_TITLEBAR_H,
        ui_ALIGN_HCENTER | ui_ALIGN_VCENTER,
        COLOR_CLEAR, COLOR_SET
    );
}

void ui_sysbar_fn_set_cell(uint8_t n, const StrItem title) {
    n %= 6;
    ui_text_area(
        font8x8_quan, title, get_frame_buffer(),
        ui_FNBAR_X(n), ui_FNBAR_Y, ui_FNBAR_W, ui_FNBAR_H,
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
