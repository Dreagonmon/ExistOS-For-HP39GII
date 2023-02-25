#include "ui_input_number.h"
#include "u8str.h"
#include "screen.h"
#include "sys_clock.h"
#include "keyboard.h"
#include "framebuf.h"
#include "ui_utils.h"
#include "ui_sysbar.h"
#include "ui_const.h"
#include "font8x8.h"
#include "font16x16.h"

#define MSG_H 32
#define MSG_Y ui_CONTENT_Y
#define MMX_H 16
#define MMX_Y (ui_CONTENT_Y + ui_CONTENT_H - MMX_H)
#define MMX_LAB_W 24
#define MMX_VAL_W (ui_CONTENT_W / 2 - MMX_LAB_W)
#define INP_Y (ui_CONTENT_Y + MSG_H)
#define INP_H (ui_CONTENT_H - MMX_H - MSG_H)

static U8StringGroup TEXTG_MIN =
    "MIN\0"
    "最小\0";
static U8StringGroup TEXTG_MAX =
    "MAX\0"
    "最大\0";
static U8StringGroup TEXTG_DEFAULT_MESSAGE =
    "Please input number\0"
    "请输入数字\0";

int32_t ui_input_number(U8String title, U8String message, int32_t min, int32_t max, int32_t init_value) {
    char text_buffer[u8_MAX_NUMBER_STRING_LENGTH] = { 0 };
    if (min <= ui_RETURN_VALUE_CANCELED) {
        // printf("Warning: user input may be seen as 'Cancel' (min is too small)");
        min = ui_RETURN_VALUE_CANCELED + 1;
    }
    // title
    gfb_fill_rect(get_frame_buffer(), 0, 0, SCR_W, SCR_H, COLOR_CLEAR);
    if (title != NULL && u8_string_size(title) > 0) {
        ui_sysbar_title(title);
    } else {
        ui_sysbar_title(ui_TEXT_EMPTY);
    }
    // min max
    ui_text_area(
        font8x8_quan, ui_trs(TEXTG_MIN), get_frame_buffer(),
        ui_CONTENT_X, MMX_Y, MMX_LAB_W, MMX_H,
        ui_ALIGN_HCENTER | ui_ALIGN_VCENTER,
        COLOR_SET, COLOR_CLEAR
    );
    i32_to_u8str(min, text_buffer);
    ui_text_area(
        font8x8_quan, text_buffer, get_frame_buffer(),
        ui_CONTENT_X + MMX_LAB_W, MMX_Y, MMX_VAL_W, MMX_H,
        ui_ALIGN_HLEFT | ui_ALIGN_VCENTER,
        COLOR_SET, COLOR_CLEAR
    );
    i32_to_u8str(max, text_buffer);
    ui_text_area(
        font8x8_quan, text_buffer, get_frame_buffer(),
        ui_CONTENT_X + ui_CONTENT_W - MMX_LAB_W - MMX_VAL_W, MMX_Y, MMX_VAL_W, MMX_H,
        ui_ALIGN_HRIGHT | ui_ALIGN_VCENTER,
        COLOR_SET, COLOR_CLEAR
    );
    ui_text_area(
        font8x8_quan, ui_trs(TEXTG_MAX), get_frame_buffer(),
        ui_CONTENT_X + ui_CONTENT_W - MMX_LAB_W, MMX_Y, MMX_LAB_W, MMX_H,
        ui_ALIGN_HCENTER | ui_ALIGN_VCENTER,
        COLOR_SET, COLOR_CLEAR
    );
    // message
    if (message != NULL && u8_string_size(message) > 0) {
        ui_text_area(
            font16x16_unifont, message, get_frame_buffer(),
            ui_CONTENT_X, MSG_Y, ui_CONTENT_W, MSG_H,
            ui_ALIGN_HCENTER | ui_ALIGN_VCENTER,
            COLOR_SET, COLOR_CLEAR
        );
    } else {
        ui_text_area(
            font16x16_unifont, ui_trs(TEXTG_DEFAULT_MESSAGE), get_frame_buffer(),
            ui_CONTENT_X, MSG_Y, ui_CONTENT_W, MSG_H,
            ui_ALIGN_HCENTER | ui_ALIGN_VCENTER,
            COLOR_SET, COLOR_CLEAR
        );
    }
    // sysbar
    ui_sysbar_fn_clear();
    ui_sysbar_fn_set_cell(0, ui_trs(ui_TEXTG_CANCEL));
    // text
    if (init_value != ui_NO_INIT_VALUE) {
        i32_to_u8str(init_value, text_buffer);
    } else {
        text_buffer[0] = '\0';
    }
    // process keyboard event
    int32_t input_value = 0;
    uint32_t kbd_event;
    uint16_t key_code;
    uint8_t flag_value_changed = 1; // screen_flush();
    uint8_t flag_checked = 0;
    while (1) {
        kbd_event = kbd_query_event();
        key_code = kbd_value(kbd_event);
        if (kbd_action(kbd_event) == kbd_ACTION_DOWN) {
            if ((key_code == kbd_K_F1 || key_code == kbd_K_ON)) {
                return ui_RETURN_VALUE_CANCELED;
            } else if ((key_code == kbd_K_F6 || key_code == kbd_K_ENTER)) {
                if (flag_checked && !flag_value_changed) {
                    return input_value;
                }
            } else if (key_code == kbd_K_BACKSPACE) {
                U8Size len = u8_string_size(text_buffer);
                if (len > 0) {
                    text_buffer[len - 1] = '\0';
                    flag_value_changed = 1;
                }
            } else if (key_code == kbd_K_PLUS) {
                U8Size len = u8_string_size(text_buffer);
                if (len > 0 && text_buffer[0] == '-' && len < u8_MAX_NUMBER_STRING_LENGTH) {
                    uint8_t i = 0;
                    while (i < len) {
                        text_buffer[i] = text_buffer[i + 1];
                        i ++;
                    }
                    flag_value_changed = 1;
                }
            } else if (key_code == kbd_K_SUBTRACTION) {
                U8Size len = u8_string_size(text_buffer);
                if ((len > 0 && text_buffer[0] != '-' && text_buffer[0] != '0' && len < (u8_MAX_NUMBER_STRING_LENGTH - 1)) || len == 0) {
                    // '-' can't add before '0'
                    text_buffer[len + 1] = '\0';
                    while (len > 0) {
                        text_buffer[len] = text_buffer[len - 1];
                        len --;
                    }
                    text_buffer[len] = '-';
                    flag_value_changed = 1;
                }
            } else if (key_code == kbd_K_UP || key_code == kbd_K_DOWN) {
                input_value = 0;
                u8str_to_i32(text_buffer, &input_value);
                if (key_code == kbd_K_UP) {
                    input_value += 1;
                } else {
                    input_value -= 1;
                }
                if ((input_value >= min) && (input_value <= max)) {
                    i32_to_u8str(input_value, text_buffer);
                    flag_value_changed = 1;
                }
            } else {
                uint8_t num = ui_get_key_number(key_code);
                if (num < 10) {
                    U8Size len = u8_string_size(text_buffer);
                    if ((len == 1 && (text_buffer[0] == '0' || text_buffer[0] == '-'))) {
                        // '0' and '-', next number can't be '0'
                        uint8_t offset = (text_buffer[0] == '0') ? 0 : 1;
                        if (num > 0) {
                            text_buffer[offset] = '0' + num;
                            text_buffer[offset + 1] = '\0';
                            flag_value_changed = 1;
                        }
                    } else if (len < (u8_MAX_NUMBER_STRING_LENGTH - 1)) {
                        text_buffer[len] = '0' + num;
                        text_buffer[len + 1] = '\0';
                        flag_value_changed = 1;
                    }
                }
            }
        }
        if (flag_value_changed) {
            // check
            flag_checked = 0;
            if (u8str_to_i32(text_buffer, &input_value)) {
                U8Size len = u8_string_size(text_buffer);
                if ((input_value >= min) && (input_value <= max)) {
                    flag_checked = 1;
                }
            }
            if (flag_checked) {
                ui_sysbar_fn_set_cell(5, ui_trs(ui_TEXTG_CONFIRM));
            } else {
                ui_sysbar_fn_set_cell(5, ui_TEXT_EMPTY);
            }
            // render number
            ui_text_area(
                font16x16_unifont, text_buffer, get_frame_buffer(),
                ui_CONTENT_X, INP_Y, ui_CONTENT_W, INP_H,
                ui_ALIGN_HCENTER | ui_ALIGN_VCENTER,
                COLOR_SET, COLOR_CLEAR
            );
            screen_flush();
            flag_value_changed = 0;
        }
        if (kbd_action(kbd_event) != kbd_ACTION_DOWN) {
            sleep_ms(30);
        }
    }
}
