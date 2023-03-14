#include <string.h>
#include "ui_file_selector.h"
#include "ui_utils.h"
#include "ui_menu.h"
#include "ui_const.h"
#include "fs_utils.h"
#include "u8str.h"
#include "FreeRTOS.h"

#include "debug.h"

void ui_file_select(U8String title, U8String dir_path) {
    char *names = pvPortMalloc(4096);
    uint8_t successed = list_dir(dir_path, names, 4096);
    if (successed) {
        U8Size file_count = u8_string_group_size(names);
        for (U8Size i = 0; i < file_count; i++) {
            printf("File: %s\n", u8_string_group_get(names, i));
        }
    } else {
        printf("Failed to list file\n");
    }
    vPortFree(names);
}
