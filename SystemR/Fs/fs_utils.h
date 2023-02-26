#pragma once

#include <stdint.h>

#include "lfs.h"
#define FS_FLASH_PATH "/"
#define FS_TIMEOUT 3000

uint8_t mount_file_system(void);
uint8_t format_file_system(void);
lfs_t *get_fs_obj(void);
