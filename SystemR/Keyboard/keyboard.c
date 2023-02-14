#include <stdint.h>
#include "sys_llapi.h"
#include "keyboard.h"

#define kbd_MakeEvent(event, value) ((value << 16) | event)
#define kbd_raw_keycode(raw_key) (raw_key & 0xFFFF)
#define kbd_raw_is_pressed(raw_key) ((raw_key >> 16) > 0)

static uint32_t last_key_value = 0;

uint8_t kbd_check_key(uint16_t key_to_check) {
    uint32_t keys = ll_vm_check_key();
    uint16_t key, press;
    if (kbd_raw_keycode(keys) == key_to_check && kbd_raw_is_pressed(keys)) {
        return 1;
    } else {
        return 0;
    }
}

uint32_t kbd_query_event() {
    uint32_t key_value = ll_vm_check_key();
    if (key_value == last_key_value) {
        return kbd_MakeEvent(kbd_ACTION_NOP, 0);
    }
    last_key_value = key_value;
    if (kbd_raw_is_pressed(key_value)) {
        return kbd_MakeEvent(kbd_ACTION_DOWN, kbd_raw_keycode(key_value));
    } else {
        return kbd_MakeEvent(kbd_ACTION_UP, kbd_raw_keycode(key_value));
    }
}
