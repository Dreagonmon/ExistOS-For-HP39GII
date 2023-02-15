#include <stdint.h>
#include "sys_llapi.h"
#include "keyboard.h"

#define kbd_MakeEvent(event, value) ((value << 16) | event)
#define kbd_raw_keycode(raw_key) (raw_key & 0xFFFF)
#define kbd_raw_is_pressed(raw_key) ((raw_key >> 16) > 0)
#define kbd_raw_pack(code, pressed) ((pressed << 16) | (code & 0xFFFF))

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
    uint16_t code = kbd_raw_keycode(key_value);
    uint16_t pressed = kbd_raw_is_pressed(key_value);
    uint16_t last_code = kbd_raw_keycode(last_key_value);
    uint16_t last_pressed = kbd_raw_is_pressed(last_key_value);
    // Only One Key Can Be Pressed At One Time.
    if (pressed && last_pressed && code != last_code) {
        last_key_value = kbd_raw_pack(last_code, 0);
        return kbd_MakeEvent(kbd_ACTION_UP, last_code);
    } else if (!pressed && last_pressed && code == last_code) {
        last_key_value = key_value;
        return kbd_MakeEvent(kbd_ACTION_UP, code);
    } else if (pressed && !last_pressed) {
        last_key_value = key_value;
        return kbd_MakeEvent(kbd_ACTION_DOWN, code);
    }
    return kbd_MakeEvent(kbd_ACTION_NOP, 0);
}
