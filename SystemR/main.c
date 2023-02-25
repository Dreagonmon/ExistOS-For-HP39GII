#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "FreeRTOS.h"
#include "fs_utils.h"
#include "sys_settings.h"
#include "sys_llapi.h"
// ui
#include "screen.h"
#include "keyboard.h"
#include "u8str.h"
#include "ui_const.h"
#include "ui_utils.h"
#include "ui_sysbar.h"
#include "ui_dialog.h"
#include "font16x16.h"
#include "font8x8.h"
#include "sys_clock.h"
// apps
#include "Settings/app_settings.h"

/* Const Text Define */
static U8StringGroup TEXTG_NOTICE_FS_NEED_FORMAT =
    "Failed to init Flash.\nDo you want to format it?\0"
    "初始化Flash存储失败.\n你想要格式化它吗?\0";
static U8StringGroup TEXTG_NOTICE_DATA_LOST =
    "Are you sure?\nAll your data will lose.\0"
    "你确定吗?\n所有的数据都会丢失.\0";
static U8StringGroup TEXTG_FORMAT_FAILED =
    "Failed to format Flash.\0"
    "Flash格式化失败.\0";
static U8StringGroup TEXTG_WELCOME_MESSAGE =
    "HP 39gII UselessOS\0"
    "HP 39gII 天地無用OS\0";
static U8StringGroup TEXTG_OFF =
    "OFF\0"
    "关机\0";
static U8StringGroup TEXTG_APPS =
    "Apps\0"
    "应用\0";
static U8StringGroup TEXTG_SETTINGS =
    "Settings\0"
    "设置\0";
static U8StringGroup TEXTG_CHARGING1 =
    "= USB Charging Enabled =\0"
    "= 已启用USB充电 =\0";

/* System Init */
static void main_init() {
    // init screen
    screen_init_mono();
    // init file system
    if (!mount_file_system()) {
        if (ui_dialog_confirm(ui_trs(ui_TEXTG_WARNING), ui_trs(TEXTG_NOTICE_FS_NEED_FORMAT))) {
            if (ui_dialog_confirm(ui_trs(ui_TEXTG_WARNING), ui_trs(TEXTG_NOTICE_DATA_LOST))) {
                if (!format_file_system() || !mount_file_system()) {
                    ui_dialog_alert(ui_trs(ui_TEXTG_ERROR), ui_trs(TEXTG_FORMAT_FAILED));
                    ll_power_off();
                }
            } else {
                ll_power_off();
            }
        } else {
            ll_power_off();
        }
    }
    // init system settings
    if (!init_settings()) {
        // default global settings
        init_default_settings();
    }
    ui_set_lang(sys_settings->ui_lang);
    // allow auto slowdown
    ll_cpu_slowdown_enable(sys_settings->cpu_slowdown_mode);
    // charge
    if (sys_settings->flag1 & sys_FLAG1_ENABLE_CHARGE_AT_BOOT) {
        ll_charge_enable(true);
    }
    // init end
}

static void render_content() {
    gfb_fill_rect(get_frame_buffer(), ui_CONTENT_X, ui_CONTENT_Y, ui_CONTENT_W, ui_CONTENT_H, COLOR_CLEAR);
    int16_t off_y = 0;
    ui_text_area(
        font16x16_unifont, ui_trs(TEXTG_WELCOME_MESSAGE), get_frame_buffer(),
        ui_CONTENT_X, ui_CONTENT_Y + off_y, ui_CONTENT_W, ui_CONTENT_H - font8x8_quan->char_height,
        ui_ALIGN_HCENTER | ui_ALIGN_VCENTER,
        COLOR_SET, COLOR_CLEAR
    );
    off_y += ui_CONTENT_H - font8x8_quan->char_height;
    if (ll_get_charge_status()) {
        ui_text_area(
            font8x8_quan, ui_trs(TEXTG_CHARGING1), get_frame_buffer(),
            ui_CONTENT_X, ui_CONTENT_Y + off_y, ui_CONTENT_W, font8x8_quan->char_height,
            ui_ALIGN_HCENTER | ui_ALIGN_VCENTER,
            COLOR_SET, COLOR_CLEAR
        );
        off_y += font8x8_quan->char_height;
    }
}

static void main_ui() {
    ui_sysbar_title("HP 39gII");
    render_content();
    ui_sysbar_fn_clear();
    ui_sysbar_fn_set_cell(0, ui_trs(TEXTG_OFF));
    ui_sysbar_fn_text(1, 4, ui_trs(TEXTG_APPS));
    ui_sysbar_fn_set_cell(5, ui_trs(TEXTG_SETTINGS));
    screen_flush();
    // test
}

void main() {
    main_init();
    main_ui();
    while (1) {
        uint32_t kevt = kbd_query_event();
        if (kbd_action(kevt) != kbd_ACTION_NOP) {
            printf("kevent: %u %u\n", kbd_action(kevt), kbd_value(kevt));
        }
        if (kbd_action(kevt) == kbd_ACTION_DOWN) {
            uint16_t kode = kbd_value(kevt);
            if (kode == kbd_K_F1 || (kode == kbd_K_ON && check_indicator(INDICATE_LEFT))) {
                ll_power_off();
                ll_power_off();
            } else if (kode == kbd_K_F6) {
                app_run_settings();
                main_ui();
            } else if (kode == kbd_K_SHIFT) {
                set_indicator(INDICATE_LEFT, (!check_indicator(INDICATE_LEFT)));
            } else {
                if (check_indicator(INDICATE_LEFT)) {
                    set_indicator(INDICATE_LEFT, (!check_indicator(INDICATE_LEFT)));
                }
            }
        } else {
            sleep_ms(30);
        }
    }
}