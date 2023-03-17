#include <string.h>
#include "ui_file_selector.h"
#include "ui_utils.h"
#include "ui_dialog.h"
#include "ui_menu.h"
#include "ui_const.h"
#include "fs_utils.h"
#include "u8str.h"
#include "FreeRTOS.h"

#define FILE_LIST_BUFFER_SIZE 8192

U8StringGroup TEXTG_FAILED_LIST_FILES =
    "Failed to list file.\0"
    "获取文件列表失败.\0";
U8StringGroup TEXTG_PATH_TOO_LONG =
    "File path is too long.\0"
    "文件路径过长.\0";

uint8_t ui_file_select(U8String title, U8String init_dir_path, char *path_buffer, const U8Size buffer_size, uint8_t can_select_file, uint8_t can_select_dir) {
    U8String part_parent = "..";
    if (buffer_size < 2) return 0;
    if (is_dir(init_dir_path)) {
        if (buffer_size < u8_string_size(init_dir_path) + 1) return 0;
        path_buffer[0] = '\0';
        strcat(path_buffer, init_dir_path);
    } else {
        path_buffer[0] = '/';
        path_buffer[1] = '\0';
    }
    U8String part = NULL;
    int16_t sel = -1;
    uint8_t ret = 0;
    uint8_t successed = 0;
    char *names = pvPortMalloc(FILE_LIST_BUFFER_SIZE);
    successed = list_dir(path_buffer, names, FILE_LIST_BUFFER_SIZE, can_select_file, can_select_dir);
    while (1) {
        if (!successed) {
            ui_dialog_alert(ui_trs(ui_TEXTG_ERROR), ui_trs(TEXTG_FAILED_LIST_FILES));
            break;
        }
        sel = ui_menu_select1(title, names, sel);
        if (sel < 0) {
            if (path_buffer[0] == '/' && path_buffer[1] == '\0') {
                // at root dir, cancel means cancel.
                break;
            }
            part = part_parent;
        } else {
            part = u8_string_group_get(names, sel);
            if (u8_string_size(part) == 1 && part[0] == '.') {
                if (can_select_dir) {
                    // select dir
                    ret = 1;
                    break;
                } else {
                    // can't select dir
                    continue;
                }
            }
        }
        successed = path_append(path_buffer, buffer_size, part);
        if (!successed) {
            ui_dialog_alert(ui_trs(ui_TEXTG_ERROR), ui_trs(TEXTG_PATH_TOO_LONG));
            successed = 1; // clear error state.
            continue;
        }
        if (is_dir(path_buffer)) {
            // enter dir
            sel = -1;
            successed = list_dir(path_buffer, names, FILE_LIST_BUFFER_SIZE, can_select_file, can_select_dir);
            continue;
        } else {
            if (can_select_file) {
                // select file
                ret = 1;
                break;
            } else {
                // can't select file
                // undo append
                successed = path_append(path_buffer, buffer_size, part_parent);
                continue;
            }
        }
    }
    vPortFree(names);
    return ret;
}
