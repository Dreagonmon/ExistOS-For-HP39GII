#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "sys_llapi.h"
#include "keyboard.h"
#include "fs_utils.h"
#include "sys_settings.h"

void display() {
    // screen_flush();
}

/* System Init */
void main_init() {
    // init screen
    screen_init_mono();
    // init file system
    mount_file_system();
    // init system settings
    if (!init_settings()) {
        // default global settings
        init_default_settings();
    }
}

void main() {
    main_init();
    // test below
    printf("Hello Dragon\n");
    printf("================================================\n");
    display();
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