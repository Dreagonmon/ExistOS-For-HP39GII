#include "app_settings.h"
#include "u8str.h"
#include "ui_utils.h"
#include "ui_menu.h"
#include "ui_input_number.h"
#include "ui_const.h"
#include "ui_dialog.h"
#include "ui_sysbar.h"
#include "sys_settings.h"
#include "sys_llapi.h"
#include "font16x16.h"
#include "build_timestamp.h"
#include "screen.h"
#include "keyboard.h"
#include "sys_clock.h"
#include "uclock.h"
#include "FreeRTOS.h"

static U8String BUILD_TIME = _BUILD_TIME_;//__DATE__ " " __TIME__;

static U8StringGroupList MENUG_SETTINGS =
    "UI Language\0"
    "Date and Time\0"
    "CPU Slowdown\0"
    "USB Charging\0"
    "System Information\0"
    "\0"
    "界面语言\0"
    "时间和日期\0"
    "CPU降频\0"
    "USB充电\0"
    "系统信息\0"
    "\0";
static U8StringGroupList MENUG_SLOWDOWNS =
    "Normal\0"
    "Slow\0"
    "Slower\0"
    "\0"
    "正常\0"
    "慢速\0"
    "更慢\0"
    "\0";
static U8StringGroupList MENUG_CHARGING =
    "Disabled\0"
    "Enable once\0"
    "Enable at boot\0"
    "\0"
    "禁用\0"
    "启用一次\0"
    "开机时启用\0"
    "\0";
static U8StringGroup MENU_TIMEZONE_OFFSET =
    "-12\0"
    "-11\0"
    "-10\0"
    "-9\0"
    "-8\0"
    "-7\0"
    "-6\0"
    "-5\0"
    "-4\0"
    "-3\0"
    "-2\0"
    "-1\0"
    "0\0"
    "+1\0"
    "+2\0"
    "+3\0"
    "+4\0"
    "+5\0"
    "+6\0"
    "+7\0"
    "+8\0"
    "+9\0"
    "+10\0"
    "+11\0"
    "+12\0";
static U8StringGroup MENU_LANGUAGES =
    "English\0"
    "简体中文\0";

static U8StringGroupList TEXTGL_DATETIME_TITLES =
    "Year\0"
    "Month\0"
    "Day of the Month\0"
    "Hour\0"
    "Minute\0"
    "Second\0"
    "\0"
    "年份\0"
    "月份\0"
    "日期\0"
    "小时\0"
    "分钟\0"
    "秒钟\0"
    "\0";
static U8StringGroup TEXTG_SETTINGS_TITLE = 
    "Settings\0"
    "设置\0";
static U8StringGroup TEXTG_CHARGING_WARNING =
    "USB Charging is an\nexperimental feature,\nUse at your own risk.\nDO NOT charge\nnormal batteries!!!\0"
    "USB充电是实验性功能, \n使用它责任自负.\n不要给普通电池充电!!!\0";
static U8StringGroup TEXTG_BUILD_TIME =
    "Build Time\0"
    "编译时间\0";
static U8StringGroup TEXTG_TIMEZONE_OFFSET =
    "Timezone Offset (Hour)\0"
    "时区偏移 (小时)\0";
static U8StringGroup TEXTG_CONFIRM_SET_DATETIME =
    "Confirm to set\nthe date and time.\0"
    "确认以设置时间和日期.\0";

static void change_language(int16_t sel) {
    int16_t lang = sys_settings->ui_lang;
    lang = (lang > 1) ? 1 : lang; // max 1
    lang = ui_menu_select2(u8_string_group_get(ui_trsg(MENUG_SETTINGS), sel), MENU_LANGUAGES, lang);
    if (lang >= 0) {
        sys_settings->ui_lang = lang & UINT8_MAX;
        ui_set_lang(lang & UINT8_MAX);
        save_settings();
    }
}

static void change_date_and_time(int16_t sel) {
    struct utm *tm = pvPortMalloc(sizeof(struct utm));
    struct utm *new_tm = pvPortMalloc(sizeof(struct utm));
    uclock_secs_to_tm(rtc_time_local(), tm);
    uint8_t loop = 1;
    int16_t sel_tz_off = sys_settings->timezone_offset + 12;
    if (sel_tz_off >= 25 || sel_tz_off < 0) sel_tz_off = 12;
    while (loop) {
        sel_tz_off = ui_menu_select2(ui_trs(TEXTG_TIMEZONE_OFFSET), MENU_TIMEZONE_OFFSET, sel_tz_off);
        if (sel_tz_off < 0) break;
        new_tm->tm_year = tm->tm_year + 1900;
        while (loop) {
            new_tm->tm_year = ui_input_number(ui_TEXT_EMPTY, u8_string_group_get(ui_trsg(TEXTGL_DATETIME_TITLES), 0), 1900, 9999, new_tm->tm_year);
            if (new_tm->tm_year == ui_RETURN_VALUE_CANCELED) break;
            new_tm->tm_mon = tm->tm_mon + 1;
            while (loop) {
                new_tm->tm_mon = ui_input_number(ui_TEXT_EMPTY, u8_string_group_get(ui_trsg(TEXTGL_DATETIME_TITLES), 1), 1, 12, new_tm->tm_mon);
                if (new_tm->tm_mon == ui_RETURN_VALUE_CANCELED) break;
                new_tm->tm_mday = tm->tm_mday;
                while (loop) {
                    new_tm->tm_mday = ui_input_number(ui_TEXT_EMPTY, u8_string_group_get(ui_trsg(TEXTGL_DATETIME_TITLES), 2), 1, 31, new_tm->tm_mday);
                    if (new_tm->tm_mday == ui_RETURN_VALUE_CANCELED) break;
                    new_tm->tm_hour = tm->tm_hour;
                    while (loop) {
                        new_tm->tm_hour = ui_input_number(ui_TEXT_EMPTY, u8_string_group_get(ui_trsg(TEXTGL_DATETIME_TITLES), 3), 0, 23, new_tm->tm_hour);
                        if (new_tm->tm_hour == ui_RETURN_VALUE_CANCELED) break;
                        new_tm->tm_min = tm->tm_min;
                        while (loop) {
                            new_tm->tm_min = ui_input_number(ui_TEXT_EMPTY, u8_string_group_get(ui_trsg(TEXTGL_DATETIME_TITLES), 4), 0, 59, new_tm->tm_min);
                            if (new_tm->tm_min == ui_RETURN_VALUE_CANCELED) break;
                            new_tm->tm_sec = tm->tm_sec;
                            while (loop) {
                                new_tm->tm_sec = ui_input_number(ui_TEXT_EMPTY, u8_string_group_get(ui_trsg(TEXTGL_DATETIME_TITLES), 5), 0, 59, new_tm->tm_sec);
                                if (new_tm->tm_sec == ui_RETURN_VALUE_CANCELED) break;
                                new_tm->tm_year -= 1900;
                                new_tm->tm_mon -= 1;
                                int8_t tz_offset = (sel_tz_off - 12);
                                int64_t tms = uclock_tm_to_secs(new_tm);
                                if (tz_offset < 0) {
                                    tms = tms + (3600 * (-tz_offset));
                                } else {
                                    tms = tms - (3600 * tz_offset);
                                }
                                if (ui_dialog_confirm(ui_trs(ui_TEXTG_INFO), ui_trs(TEXTG_CONFIRM_SET_DATETIME))) {
                                    if (g_system_in_emulator) {
                                        sys_settings->timezone_offset = 0;
                                        sys_settings->rtc_offset = (tms - ll_rtc_get_sec());
                                    } else {
                                        ll_rtc_set_sec(0);
                                        sys_settings->timezone_offset = tz_offset;
                                        sys_settings->rtc_offset = tms & INT64_MAX;
                                    }
                                    save_settings();
                                    loop = 0;
                                    break;
                                } else {
                                    new_tm->tm_year += 1900;
                                    new_tm->tm_mon += 1;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    vPortFree(tm);
    vPortFree(new_tm);
}

static void change_cpu_slowdown(int16_t sel) {
    int16_t sld = sys_settings->cpu_slowdown_mode;
    sld = (sld > 2) ? 2 : sld; // max 2
    sld = ui_menu_select2(u8_string_group_get(ui_trsg(MENUG_SETTINGS), sel), ui_trsg(MENUG_SLOWDOWNS), sld);
    if (sld >= 0) {
        sys_settings->cpu_slowdown_mode = sld & UINT8_MAX;
        ll_cpu_slowdown_enable(sld & UINT8_MAX);
        save_settings();
    }
}

static void change_chagring(int16_t sel) {
    while (1) {
        int16_t chg = (sys_settings->flag1 & sys_FLAG1_ENABLE_CHARGE_AT_BOOT) ? 2 : 0;
        if (chg == 0) {
            chg = ll_get_charge_status() ? 1 : 0;
        }
        chg = ui_menu_select2(u8_string_group_get(ui_trsg(MENUG_SETTINGS), sel), ui_trsg(MENUG_CHARGING), chg);
        if (chg < 0) break;
        if (chg >= 1) { // enable charge
            if (ui_dialog_confirm(ui_trs(ui_TEXTG_WARNING), ui_trs(TEXTG_CHARGING_WARNING))) {
                ll_charge_enable(1);
            } else {
                continue;
            }
        } else {
            ll_charge_enable(0);
        }
        if (chg >= 2) { // enable charge at boot
            sys_settings->flag1 = sys_settings->flag1 | sys_FLAG1_ENABLE_CHARGE_AT_BOOT;
        } else {
            sys_settings->flag1 = sys_settings->flag1 & (~sys_FLAG1_ENABLE_CHARGE_AT_BOOT);
        }
        save_settings();
        break;
    }
}

static void ui_about(int16_t sel) {
    ui_sysbar_title(u8_string_group_get(ui_trsg(MENUG_SETTINGS), sel));
    gfb_fill_rect(get_frame_buffer(), ui_CONTENT_X, ui_CONTENT_Y, ui_CONTENT_W, ui_CONTENT_H, COLOR_CLEAR);
    int16_t off_y = ui_CONTENT_Y;
    ui_text_area(
        font16x16_unifont, ui_trs(TEXTG_BUILD_TIME), get_frame_buffer(),
        ui_CONTENT_X, off_y, ui_CONTENT_W, font16x16_unifont->char_height,
        ui_ALIGN_HCENTER | ui_ALIGN_VCENTER, COLOR_SET, COLOR_CLEAR
    );
    off_y += font16x16_unifont->char_height;
    ui_text_area(
        font16x16_unifont, BUILD_TIME, get_frame_buffer(),
        ui_CONTENT_X, off_y, ui_CONTENT_W, font16x16_unifont->char_height,
        ui_ALIGN_HCENTER | ui_ALIGN_VCENTER, COLOR_SET, COLOR_CLEAR
    );
    ui_sysbar_fn_text(0, 6, ui_trs(ui_TEXTG_OK));
    screen_flush();
    // waiting for input
    uint32_t kbd_event;
    uint16_t key_code;
    while (1) {
        kbd_event = kbd_query_event();
        if (kbd_action(kbd_event) == kbd_ACTION_DOWN) {
            key_code = kbd_value(kbd_event);
            if (key_code == kbd_K_F1 || key_code == kbd_K_F2 || key_code == kbd_K_F3 || key_code == kbd_K_F4 || key_code == kbd_K_F5 || key_code == kbd_K_F6 || key_code == kbd_K_ON || key_code == kbd_K_ENTER) {
                return;
            }
        } else {
            sleep_ms(30);
        }
    }
}

void app_run_settings() {
    int16_t sel = -1;
    while (1) {
        sel = ui_menu_select2(ui_trs(TEXTG_SETTINGS_TITLE), ui_trsg(MENUG_SETTINGS), sel);
        if (sel < 0) {
            return;
        } else if (sel == 0) {
            // UI Language
            change_language(sel);
        } else if (sel == 1) {
            // Date and Time
            change_date_and_time(sel);
        } else if (sel == 2) {
            // CPU Slowdown
            change_cpu_slowdown(sel);
        } else if (sel == 3) {
            // USB Charging
            change_chagring(sel);
        } else if (sel == 4) {
            // System Information
            ui_about(sel);
        }
    }
}
