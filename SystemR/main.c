#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "sys_llapi.h"
#include "screen.h"
#include "keyboard.h"
#include "fs_utils.h"
#include "sys_settings.h"
// ui
#include "u8str.h"
#include "ui_const.h"
#include "ui_utils.h"
#include "ui_dialog.h"

/* Const Text Define */
U8StringGroup TEXTG_NOTICE_FS_NEED_FORMAT =
    "Failed to init Flash.\nDo you want to format it?\0"
    "初始化Flash存储失败.\n你想要格式化它吗?\0";
U8StringGroup TEXTG_NOTICE_DATA_LOST =
    "Are you sure?\nAll your data will lose.\0"
    "你确定吗?\n所有的数据都会丢失.\0";
U8StringGroup TEXTG_FORMAT_FAILED =
    "Failed to format Flash.\0"
    "Flash格式化失败.\0";

/* System Init */
void main_init() {
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
    // init end
}

void display() {
    // test
    ui_set_lang(ui_LANG_CHS);
    uint8_t rst = ui_dialog_confirm("对话框", "请选择是否继续执行");
    if (rst) {
        ui_dialog_alert("操作结果", "你选择了继续执行");
    } else {
        ui_dialog_alert("操作结果", "你取消了执行");
    }
}

void main() {
    main_init();
    // test below
    display();
    printf("Hello Dragon\n");
    printf("================================================\n");
    ll_charge_enable(true);
    while (1) {
        uint32_t kevt = kbd_query_event();
        if (kbd_action(kevt) != kbd_ACTION_NOP) {
            printf("kevent: %u %u\n", kbd_action(kevt), kbd_value(kevt));
        }
        if (kbd_check_key(kbd_K_ON)) {
            ll_power_off();
        }
    }
}