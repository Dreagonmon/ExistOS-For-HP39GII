#pragma once

#include <stdint.h>
#include "lfs.h"
#include "u8str.h"

#define FS_FLASH_PATH "/"
#define FS_TIMEOUT 3000

uint8_t mount_file_system(void);
uint8_t format_file_system(void);
lfs_t *get_fs_obj(void);

// High Level FS Operations
/** 
 * @param[in] dir_path U8String dir path
 * @param[out] out_buffer U8StringGroup format file list
 * @param[in] len out_buffer size
 * @return U8Size bytes changed in 'out_buffer' (include all '\0')
 * */
U8Size list_dir(U8String dir_path, char *out_buffer, U8Size len);
