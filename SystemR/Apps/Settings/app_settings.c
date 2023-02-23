#include "app_settings.h"
#include "u8str.h"
#include "ui_utils.h"
#include "ui_menu.h"
#include "ui_const.h"
#include "ui_dialog.h"
#include "sys_settings.h"
#include "sys_llapi.h"

static U8StringGroupList MENUG_SETTINGS =
    "UI Language\0"
    "Date and Time\0"
    "CPU Slowdown\0"
    "USB Charging\0"
    "\0"
    "界面语言\0"
    "时间和日期\0"
    "CPU降频\0"
    "USB充电\0"
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
static U8StringGroup MENU_LANGUAGES =
    "English\0"
    "简体中文\0";

static U8StringGroup TEXTG_SETTINGS_TITLE = 
    "Settings\0"
    "设置\0";
static U8StringGroup TEXTG_CHARGING_WARNING =
    "USB Charging is an\nexperimental feature,\nUse at your own risk.\nDO NOT charge\nnormal batteries!!!\0"
    "USB充电是实验性功能, \n使用它责任自负.\n不要给普通电池充电!!!\0";

void app_run_settings() {
    int16_t sel = -1;
    while (1) {
        sel = ui_menu_select2(ui_trs(TEXTG_SETTINGS_TITLE), ui_trsg(MENUG_SETTINGS), sel);
        if (sel < 0) {
            return;
        } else if (sel == 0) {
            // UI Language
            int16_t lang = sys_settings->ui_lang;
            lang = (lang > 1) ? 1 : lang; // max 1
            lang = ui_menu_select2(u8_string_group_get(ui_trsg(MENUG_SETTINGS), sel), MENU_LANGUAGES, lang);
            if (lang >= 0) {
                sys_settings->ui_lang = lang & UINT8_MAX;
                ui_set_lang(lang & UINT8_MAX);
                save_settings();
            }
        } else if (sel == 1) {
            // Date and Time
        } else if (sel == 2) {
            // CPU Slowdown
            int16_t sld = sys_settings->cpu_slowdown_mode;
            sld = (sld > 2) ? 2 : sld; // max 2
            sld = ui_menu_select2(u8_string_group_get(ui_trsg(MENUG_SETTINGS), sel), ui_trsg(MENUG_SLOWDOWNS), sld);
            if (sld >= 0) {
                sys_settings->cpu_slowdown_mode = sld & UINT8_MAX;
                ll_cpu_slowdown_enable(sld & UINT8_MAX);
                save_settings();
            }
        } else if (sel == 3) {
            // USB Charging
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
    }
}
