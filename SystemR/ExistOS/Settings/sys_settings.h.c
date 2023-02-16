/** Save And Load System Settings
 * 
 * file format:
 * offset | size | field
 * 0      | 2    | settings_version
 * 2      | 8    | rtc_offset
 * 10     | 1    | ui_lang
 * 11     | 1    | cpu_slowdown_mode
*/
#include "sys_settings.h"
#include "ff.h"

#define ensure_fileok(rst) ({ if (rst != FR_OK) return 0; })
#define ensure_count(wrc, expect) ({ if (wrc != expect) return 0; })
#define ensure_true(x) ({ if (!x) return 0; })

static const TCHAR SAVE_FILE_PATH[] = "/SYST.BIN";
static const uint16_t CURRENT_VERSION = 1;
struct sys_settings sys_settings_obj = { .settings_inited = 0 };
struct sys_settings *sys_settings = &sys_settings_obj;

static uint8_t write_u8(FIL *f, uint16_t val) {
    uint8_t buf[1] = {0};
    buf[0] = (val >> 0) & 0xFF;
    UINT writen = 0;
    FRESULT rst = f_write(f, buf, 1, &writen);
    ensure_fileok(rst);
    ensure_count(writen, 1);
    return 1;
}
static uint8_t write_u16(FIL *f, uint16_t val) {
    uint8_t buf[2] = {0};
    buf[0] = (val >> 8) & 0xFF;
    buf[1] = (val >> 0) & 0xFF;
    UINT writen = 0;
    FRESULT rst = f_write(f, buf, 2, &writen);
    ensure_fileok(rst);
    ensure_count(writen, 2);
    return 1;
}
static uint8_t write_u32(FIL *f, uint32_t val) {
    uint8_t buf[4] = {0};
    buf[0] = (val >> 24) & 0xFF;
    buf[1] = (val >> 16) & 0xFF;
    buf[2] = (val >> 8) & 0xFF;
    buf[3] = (val >> 0) & 0xFF;
    UINT writen = 0;
    FRESULT rst = f_write(f, buf, 4, &writen);
    ensure_fileok(rst);
    ensure_count(writen, 4);
    return 1;
}
static uint8_t write_u64(FIL *f, uint64_t val) {
    uint8_t buf[8] = {0};
    buf[0] = (val >> 56) & 0xFF;
    buf[1] = (val >> 48) & 0xFF;
    buf[2] = (val >> 40) & 0xFF;
    buf[3] = (val >> 32) & 0xFF;
    buf[4] = (val >> 24) & 0xFF;
    buf[5] = (val >> 16) & 0xFF;
    buf[6] = (val >> 8) & 0xFF;
    buf[7] = (val >> 0) & 0xFF;
    UINT writen = 0;
    FRESULT rst = f_write(f, buf, 8, &writen);
    ensure_fileok(rst);
    ensure_count(writen, 8);
    return 1;
}
static uint8_t read_u8(FIL *f, uint8_t *val) {
    uint8_t buf[1] = {0};
    UINT read = 0;
    FRESULT rst = f_read(f, buf, 1, &read);
    ensure_fileok(rst);
    ensure_count(read, 1);
    uint16_t v = 0;
    v |= (uint16_t)(buf[0]) << 0;
    *val = v;
    return 1;
}
static uint8_t read_u16(FIL *f, uint16_t *val) {
    uint8_t buf[2] = {0};
    UINT read = 0;
    FRESULT rst = f_read(f, buf, 2, &read);
    ensure_fileok(rst);
    ensure_count(read, 2);
    uint16_t v = 0;
    v |= (uint16_t)(buf[0]) << 8;
    v |= (uint16_t)(buf[1]) << 0;
    *val = v;
    return 1;
}
static uint8_t read_u32(FIL *f, uint32_t *val) {
    uint8_t buf[4] = {0};
    UINT read = 0;
    FRESULT rst = f_read(f, buf, 4, &read);
    ensure_fileok(rst);
    ensure_count(read, 4);
    uint32_t v = 0;
    v |= (uint32_t)(buf[0]) << 24;
    v |= (uint32_t)(buf[1]) << 16;
    v |= (uint32_t)(buf[2]) << 8;
    v |= (uint32_t)(buf[3]) << 0;
    *val = v;
    return 1;
}
static uint8_t read_u64(FIL *f, uint64_t *val) {
    uint8_t buf[8] = {0};
    UINT read = 0;
    FRESULT rst = f_read(f, buf, 8, &read);
    ensure_fileok(rst);
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
    FIL f;
    uint64_t val64;
    // uint32_t val32;
    uint16_t val16;
    uint8_t val8;
    uint16_t version = 0;
    ensure_fileok(f_open(&f, SAVE_FILE_PATH, FA_READ | FA_OPEN_EXISTING));
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
    ensure_fileok(f_close(&f));
    sys_settings_obj.settings_version = CURRENT_VERSION;
    sys_settings_obj.settings_inited = 1;
    return 1;
}

void init_default_settings() {
    sys_settings_obj.settings_version = CURRENT_VERSION;
    sys_settings_obj.rtc_offset = 0;
    sys_settings_obj.ui_lang = 0;
    sys_settings_obj.cpu_slowdown_mode = 0;
    sys_settings_obj.flag1 = 0;
    sys_settings_obj.flag2 = 0;
    sys_settings_obj.settings_inited = 1;
}

uint8_t save_settings() {
    FIL f;
    ensure_fileok(f_open(&f, SAVE_FILE_PATH, FA_WRITE | FA_CREATE_ALWAYS));
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
    // finished.
    ensure_fileok(f_sync(&f));
    ensure_fileok(f_close(&f));
    return 1;
}
