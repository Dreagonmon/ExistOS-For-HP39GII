/** Save And Load System Settings
 * 
 * file format:
 * offset | size | field
 * 0      | 2    | settings_version
 * 2      | 8    | rtc_offset
 * 10     | 1    | ui_lang
 * 11     | 1    | cpu_slowdown_mode
 * 12     | 1    | flag1
 * 13     | 1    | flag2
 * 14     | 1    | timezone_offset
*/
#include "sys_settings.h"
#include "fs_utils.h"
#include "lfs.h"

// used in single read and write
#define ensure_writeok(rst) ({ if (rst < 0) return 0; })
#define ensure_readok(rst) ({ if (rst < 0) return 0; })
#define ensure_count(wrc, expect) ({ if (wrc != expect) return 0; })
// used in functions
#define ensure_fsok(rst) ({ if (rst < 0) goto failed; })
#define ensure_true(x) ({ if (!x) goto failed; })

static const char SAVE_FILE_PATH[] = "/SYSCONF.BIN";
static const uint16_t CURRENT_VERSION = 2;
struct sys_settings sys_settings_obj = { .settings_inited = 0 };
struct sys_settings *sys_settings = &sys_settings_obj;

static uint8_t write_u8(lfs_file_t *f, uint16_t val) {
    uint8_t buf[1] = {0};
    buf[0] = (val >> 0) & 0xFF;
    lfs_ssize_t writen = lfs_file_write(get_fs_obj(), f, buf, 1);
    ensure_writeok(writen);
    ensure_count(writen, 1);
    return 1;
}
static uint8_t write_u16(lfs_file_t *f, uint16_t val) {
    uint8_t buf[2] = {0};
    buf[0] = (val >> 8) & 0xFF;
    buf[1] = (val >> 0) & 0xFF;
    lfs_ssize_t writen = lfs_file_write(get_fs_obj(), f, buf, 2);
    ensure_writeok(writen);
    ensure_count(writen, 2);
    return 1;
}
static uint8_t write_u32(lfs_file_t *f, uint32_t val) {
    uint8_t buf[4] = {0};
    buf[0] = (val >> 24) & 0xFF;
    buf[1] = (val >> 16) & 0xFF;
    buf[2] = (val >> 8) & 0xFF;
    buf[3] = (val >> 0) & 0xFF;
    lfs_ssize_t writen = lfs_file_write(get_fs_obj(), f, buf, 4);
    ensure_writeok(writen);
    ensure_count(writen, 4);
    return 1;
}
static uint8_t write_u64(lfs_file_t *f, uint64_t val) {
    uint8_t buf[8] = {0};
    buf[0] = (val >> 56) & 0xFF;
    buf[1] = (val >> 48) & 0xFF;
    buf[2] = (val >> 40) & 0xFF;
    buf[3] = (val >> 32) & 0xFF;
    buf[4] = (val >> 24) & 0xFF;
    buf[5] = (val >> 16) & 0xFF;
    buf[6] = (val >> 8) & 0xFF;
    buf[7] = (val >> 0) & 0xFF;
    lfs_ssize_t writen = lfs_file_write(get_fs_obj(), f, buf, 8);
    ensure_writeok(writen);
    ensure_count(writen, 8);
    return 1;
}
static uint8_t read_u8(lfs_file_t *f, uint8_t *val) {
    uint8_t buf[1] = {0};
    lfs_ssize_t read = lfs_file_read(get_fs_obj(), f, buf, 1);
    ensure_readok(read);
    ensure_count(read, 1);
    uint16_t v = 0;
    v |= (uint16_t)(buf[0]) << 0;
    *val = v;
    return 1;
}
static uint8_t read_u16(lfs_file_t *f, uint16_t *val) {
    uint8_t buf[2] = {0};
    lfs_ssize_t read = lfs_file_read(get_fs_obj(), f, buf, 2);
    ensure_readok(read);
    ensure_count(read, 2);
    uint16_t v = 0;
    v |= (uint16_t)(buf[0]) << 8;
    v |= (uint16_t)(buf[1]) << 0;
    *val = v;
    return 1;
}
static uint8_t read_u32(lfs_file_t *f, uint32_t *val) {
    uint8_t buf[4] = {0};
    lfs_ssize_t read = lfs_file_read(get_fs_obj(), f, buf, 4);
    ensure_readok(read);
    ensure_count(read, 4);
    uint32_t v = 0;
    v |= (uint32_t)(buf[0]) << 24;
    v |= (uint32_t)(buf[1]) << 16;
    v |= (uint32_t)(buf[2]) << 8;
    v |= (uint32_t)(buf[3]) << 0;
    *val = v;
    return 1;
}
static uint8_t read_u64(lfs_file_t *f, uint64_t *val) {
    uint8_t buf[8] = {0};
    lfs_ssize_t read = lfs_file_read(get_fs_obj(), f, buf, 8);
    ensure_readok(read);
    ensure_count(read, 8);
    uint64_t v = 0;
    v |= ((uint64_t)(buf[0]) << 56);
    v |= ((uint64_t)(buf[1]) << 48);
    v |= ((uint64_t)(buf[2]) << 40);
    v |= ((uint64_t)(buf[3]) << 32);
    v |= ((uint64_t)(buf[4]) << 24);
    v |= ((uint64_t)(buf[5]) << 16);
    v |= ((uint64_t)(buf[6]) << 8);
    v |= ((uint64_t)(buf[7]) << 0);
    *val = v;
    return 1;
}

uint8_t init_settings() {
    lfs_file_t f;
    uint64_t val64;
    // uint32_t val32;
    uint16_t val16;
    uint8_t val8;
    uint16_t version = 0;
    if (lfs_file_open(get_fs_obj(), &f, SAVE_FILE_PATH, LFS_O_RDONLY) < 0) return 0;
    // settings_version
    ensure_true(read_u16(&f, &val16));
    version = val16;
    // rtc_offset
    if (version >= 1) {
        ensure_true(read_u64(&f, &val64));
        sys_settings_obj.rtc_offset = val64;
    }
    // ui_lang
    if (version >= 1) {
        ensure_true(read_u8(&f, &val8));
        sys_settings_obj.ui_lang = val8;
    }
    // cpu_slowdown_mode
    if (version >= 1) {
        ensure_true(read_u8(&f, &val8));
        sys_settings_obj.cpu_slowdown_mode = val8;
    }
    // flag1
    if (version >= 1) {
        ensure_true(read_u8(&f, &val8));
        sys_settings_obj.flag1 = val8;
    }
    // flag2
    if (version >= 1) {
        ensure_true(read_u8(&f, &val8));
        sys_settings_obj.flag2 = val8;
    }
    // timezone_offset
    if (version >= 2) {
        ensure_true(read_u8(&f, &val8));
        sys_settings_obj.timezone_offset = (int8_t) val8;
    } else {
        sys_settings_obj.timezone_offset = 0;
    }
    ensure_fsok(lfs_file_close(get_fs_obj(), &f));
    sys_settings_obj.settings_version = CURRENT_VERSION;
    sys_settings_obj.settings_inited = 1;
    return 1;
    // failed
    failed:
    lfs_file_close(get_fs_obj(), &f);
    return 0;
}

void init_default_settings() {
    sys_settings_obj.settings_version = CURRENT_VERSION;
    sys_settings_obj.rtc_offset = 0;
    sys_settings_obj.ui_lang = 0;
    sys_settings_obj.cpu_slowdown_mode = 0;
    sys_settings_obj.flag1 = 0;
    sys_settings_obj.flag2 = 0;
    sys_settings_obj.timezone_offset = 0;
    sys_settings_obj.settings_inited = 1;
}

uint8_t save_settings() {
    lfs_file_t f;
    if (lfs_file_open(get_fs_obj(), &f, SAVE_FILE_PATH, LFS_O_WRONLY | LFS_O_CREAT | LFS_O_TRUNC) < 0) return 0;
    // settings_version
    ensure_true(write_u16(&f, CURRENT_VERSION));
    // rtc_offset
    ensure_true(write_u64(&f, sys_settings_obj.rtc_offset));
    // ui_lang
    ensure_true(write_u8(&f, sys_settings_obj.ui_lang));
    // cpu_slowdown_mode
    ensure_true(write_u8(&f, sys_settings_obj.cpu_slowdown_mode));
    // flag1
    ensure_true(write_u8(&f, sys_settings_obj.flag1));
    // flag2
    ensure_true(write_u8(&f, sys_settings_obj.flag2));
    // timezone_offset
    ensure_true(write_u8(&f, sys_settings_obj.timezone_offset));
    // finished.
    ensure_fsok(lfs_file_close(get_fs_obj(), &f));
    return 1;
    // failed
    failed:
    lfs_file_close(get_fs_obj(), &f);
    return 0;
}
