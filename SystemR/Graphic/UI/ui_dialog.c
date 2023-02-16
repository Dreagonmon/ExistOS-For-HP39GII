#include "ui_dialog.h"
#include "ui_sysbar.h"
#include "ui_utils.h"
#include "ui_const.h"
#include "font16x16.h"
#include "screen.h"
#include "keyboard.h"
#include "sys_clock.h"

uint8_t __ui_dialog(U8String title, U8String content, uint8_t is_alert) {
    uint16_t offy = ui_TITLEBAR_Y;
    uint16_t h = ui_TITLEBAR_H + ui_CONTENT_H;
    if (title != NULL && u8_string_size(title) > 0) {
        offy = ui_CONTENT_Y;
        h = ui_CONTENT_H;
        ui_sysbar_title(title);
    }
    if (content != NULL && u8_string_size(content) > 0) {
        ui_text_area(
            font16x16_unifont, content, get_frame_buffer(),
            ui_CONTENT_X, offy, ui_CONTENT_W, h,
            ui_ALIGN_HCENTER | ui_ALIGN_VCENTER,
            COLOR_SET, COLOR_CLEAR
        );
    } else {
        ui_text_area(
            font16x16_unifont, ui_TEXT_EMPTY, get_frame_buffer(),
            ui_CONTENT_X, offy, ui_CONTENT_W, h,
            ui_ALIGN_HCENTER | ui_ALIGN_VCENTER,
            COLOR_SET, COLOR_CLEAR
        );
    }
    ui_sysbar_fn_clear();
    if (is_alert) {
        ui_sysbar_fn_text(0, 6, ui_trs(ui_TEXTG_OK));
    } else {
        ui_sysbar_fn_set_cell(0, ui_trs(ui_TEXTG_CANCEL));
        ui_sysbar_fn_set_cell(5, ui_trs(ui_TEXTG_CONFIRM));
    }
    screen_flush();
    // waiting for input
    uint32_t kbd_event;
    uint16_t key_code;
    while (1) {
        kbd_event = kbd_query_event();
        if (kbd_action(kbd_event) == kbd_ACTION_DOWN) {
            key_code = kbd_value(kbd_event);
            if (is_alert && (key_code == kbd_K_F1 || key_code == kbd_K_F2 || key_code == kbd_K_F3 || key_code == kbd_K_F4 || key_code == kbd_K_F5 || key_code == kbd_K_F6 || key_code == kbd_K_ON || key_code == kbd_K_ENTER)) {
                return 1;
            } else if (!is_alert && (key_code == kbd_K_F1 || key_code == kbd_K_ON)) {
                return 0;
            } else if (!is_alert && (key_code == kbd_K_F6 || key_code == kbd_K_ENTER)) {
                return 1;
            }
        } else {
            sleep_ms(100);
        }
    }
}

uint8_t ui_dialog_confirm(U8String title, U8String content) {
    return __ui_dialog(title, content, 0);
}

void ui_dialog_alert(U8String title, U8String content) {
    __ui_dialog(title, content, 1);
}
