#pragma once

#include <stdint.h>
#include "u8str.h"

uint8_t ui_file_select(U8String title, U8String init_dir_path, char *path_buffer, const U8Size buffer_size, uint8_t can_select_file, uint8_t can_select_dir);
