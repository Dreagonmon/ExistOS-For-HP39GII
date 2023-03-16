#include <string.h>
#include "ui_file_selector.h"
#include "ui_utils.h"
#include "ui_menu.h"
#include "ui_const.h"
#include "fs_utils.h"
#include "u8str.h"
#include "FreeRTOS.h"

#include "debug.h"

#define FILE_LIST_BUFFER_SIZE 4096

void ui_file_select(U8String title, U8String dir_path) {
    char *path_buffer = pvPortMalloc(1024);
    path_buffer[0] = '\0';
    const U8Size path_buffer_size = 1024;
    char *names = pvPortMalloc(FILE_LIST_BUFFER_SIZE);
    uint8_t successed = list_dir(dir_path, names, FILE_LIST_BUFFER_SIZE);
    if (successed) {
        U8Size file_count = u8_string_group_size(names);
        for (U8Size i = 0; i < file_count; i++) {
            printf("File: %s\n", u8_string_group_get(names, i));
        }
    } else {
        printf("Failed to list file\n");
    }
    strcat(path_buffer, "");
    path_append(path_buffer, path_buffer_size, "DATA");
    path_append(path_buffer, path_buffer_size, "app");
    printf("PATH: %s\n", path_buffer);
    vPortFree(names);
    MEM_USED("end");
}
