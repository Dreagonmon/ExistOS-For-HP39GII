#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "FreeRTOS.h"
#include "sys_llapi.h"
#include "framebuf.h"
#include "bmfont.h"
#include "screen.h"
#include "font8x8.h"
#include "font16x16.h"
#include "ui_utils.h"
#include "ui_sysbar.h"
#include "keyboard.h"
#include "ff.h"
#include "fs_utils.h"

gfb_FrameBuffer *frame = NULL;
int16_t offx = 0;
int16_t offy = 0;

const StrItem TEXT_MENU[] = stritemlist(
    stritem("Main Menu"),
    stritem("主菜单")
);
const StrItem TEXT_F1[] = stritemlist(
    stritem("Fn1"),
    stritem("功能1")
);
const StrItem TEXT_F2[] = stritemlist(
    stritem("Fn2"),
    stritem("功能2")
);
const StrItem TEXT_F3[] = stritemlist(
    stritem("Fn3"),
    stritem("功能3")
);
const StrItem TEXT_F4[] = stritemlist(
    stritem("Fn4"),
    stritem("功能4")
);
const StrItem TEXT_F5[] = stritemlist(
    stritem("Fn5"),
    stritem("功能5")
);
const StrItem TEXT_F6[] = stritemlist(
    stritem("Fn6"),
    stritem("功能6")
);

void display() {
    ui_set_lang(ui_LANG_CHS);
    ui_sysbar_title(ui_trs(TEXT_MENU));
    ui_sysbar_fn_set_cell(0, ui_trs(TEXT_F1));
    ui_sysbar_fn_set_cell(1, ui_trs(TEXT_F2));
    // ui_sysbar_fn_set_cell(2, ui_trs(TEXT_F3));
    ui_set_lang(ui_LANG_ENG);
    // ui_sysbar_fn_set_cell(3, ui_trs(TEXT_F4));
    ui_sysbar_fn_set_cell(4, ui_trs(TEXT_F5));
    ui_sysbar_fn_set_cell(5, ui_trs(TEXT_F6));
    screen_flush();
}

/* System Init */
void main_init() {
    ll_cpu_slowdown_enable(false);
    screen_init_mono();
}

void main() {
    main_init();
    // test below
    printf("Hello Dragon\n");
    printf("================================================\n");
    display();
    ll_charge_enable(true);
    initFS();
    FIL f;
    FRESULT rst = f_open(&f, "test.txt", FA_READ | FA_WRITE | FA_CREATE_ALWAYS);
    UINT size;
    rst = f_write(&f, "Hello\n", 6, &size);
    rst = f_sync(&f);
    rst = f_close(&f);
    printf("rst %d\n", rst);
    while (1) {
        if (kbd_check_key(kbd_K_ON)) {
            ll_power_off();
        }
    }
}