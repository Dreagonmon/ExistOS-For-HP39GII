#pragma once

#include <stdint.h>
#include "lfs.h"
#include "u8str.h"

#define FS_TIMEOUT 3000
#define FS_ROOT_PATH "/"
#define FS_APP_DATA_PATH "/DATA"

uint8_t mount_file_system(void);
uint8_t format_file_system(void);
lfs_t *get_fs_obj(void);

// High Level FS Operations
/** list_dir
 * @param[in] dir_path U8String dir path.
 * @param[out] out_buffer U8StringGroup format file list.
 * @param[in] len out_buffer size.
 * @return U8Size bytes in 'out_buffer' (include all '\0'), 0 if failed (not a dir, bufer too small).
 * */
U8Size list_dir(U8String dir_path, char *out_buffer, U8Size len, uint8_t show_files, uint8_t show_this_dir);

/** path_append
 * @param[in] dest base and dest path string, must big enough.
 * @param[out] len dest buffer size.
 * @param[in] part part to be append after base path. must not contain any special characters.
 * @return U8Size bytes in 'dest' (include tail '\0'), 0 if buffer is too small. if return false, the dest is untouched.
 * */
U8Size path_append(char *dest, U8Size len, U8String part);

uint8_t path_exist(U8String path);
uint8_t is_file(U8String path);
uint8_t is_dir(U8String path);