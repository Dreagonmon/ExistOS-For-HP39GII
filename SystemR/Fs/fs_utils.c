#include <string.h>
#include "sys_llapi.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "fs_utils.h"
#include "cc_slist.h"
#include "u8str.h"

#define FLAG_CFG_INITED       0b00000001

static uint8_t flags = 0;
static lfs_t lfs;

static int _fs_read(const struct lfs_config *c, lfs_block_t block,
            lfs_off_t off, void *buffer, lfs_size_t size) {
    ll_flash_page_read(block, 1, buffer);
    return LFS_ERR_OK;
}

static int _fs_prog(const struct lfs_config *c, lfs_block_t block,
            lfs_off_t off, const void *buffer, lfs_size_t size) {
    ll_flash_page_write(block, 1, (uint8_t *)buffer);
    return LFS_ERR_OK;
}

static int _fs_erase(const struct lfs_config *c, lfs_block_t block) {
    ll_flash_page_trim(block);
    return LFS_ERR_OK;
}

static int _fs_sync(const struct lfs_config *c) {
    ll_flash_sync();
    return LFS_ERR_OK;
}

SemaphoreHandle_t xSemaphore = NULL;
StaticSemaphore_t xSemaphoreBuffer;

static int _fs_lock(const struct lfs_config *c) {
    xSemaphoreTake(xSemaphore, (FS_TIMEOUT / portTICK_PERIOD_MS));
    return LFS_ERR_OK;
}

static int _fs_unlock(const struct lfs_config *c) {
    xSemaphoreGive(xSemaphore);
    return LFS_ERR_OK;
}

struct lfs_config cfg = {
    // block device operations
    .read  = _fs_read,
    .prog  = _fs_prog,
    .erase = _fs_erase,
    .sync  = _fs_sync,
    .lock = _fs_lock,
    .unlock = _fs_unlock,

    .block_cycles = -1,
};

static void check_and_init_config() {
    if (!(flags & FLAG_CFG_INITED)) {
        // make sure read and write a complete block at a time
        cfg.read_size = ll_flash_get_page_size();
        cfg.prog_size = ll_flash_get_page_size();
        cfg.cache_size = ll_flash_get_page_size();
        cfg.lookahead_size = ll_flash_get_page_size();
        cfg.block_size = ll_flash_get_page_size();
        cfg.block_count = ll_flash_get_pages();
        cfg.read_buffer = pvPortMalloc(cfg.cache_size);
        cfg.prog_buffer = pvPortMalloc(cfg.cache_size);
        cfg.lookahead_buffer = pvPortMalloc(cfg.lookahead_size);
        xSemaphore = xSemaphoreCreateBinaryStatic(&xSemaphoreBuffer);
        xSemaphoreGive(xSemaphore);
        flags |= FLAG_CFG_INITED;
    }
}

uint8_t mount_file_system() {
    check_and_init_config();
    int err = lfs_mount(&lfs, &cfg);
    if (err == LFS_ERR_OK) printf("Filesystem mounted.\n");
    return err == LFS_ERR_OK;
}

uint8_t format_file_system() {
    int err = lfs_format(&lfs, &cfg);
    return err == LFS_ERR_OK;
}

lfs_t *get_fs_obj() {
    return &lfs;
}

static int _cc_slist_cmp_(const void *e1, const void *e2)
{
    U8String el1 = *((U8String*) e1);
    U8String el2 = *((U8String*) e2);
    return strcmp(el1, el2);
}

static void _cc_slist_free_u8str_(void * data) {
    vPortFree(data);
}

U8Size list_dir(U8String dir_path, char *out_buffer, U8Size len) {
    int ret;
    lfs_t *fs = get_fs_obj();
    lfs_dir_t *dir = &((lfs_dir_t){ 0 });
    ret = lfs_dir_open(fs, dir, dir_path);
    if (ret < 0) {
        return 0;
    }
    CC_SList *dir_list = NULL;
    CC_SList *file_list = NULL;
    cc_slist_new(&dir_list);
    cc_slist_new(&file_list);
    struct lfs_info *info = pvPortMalloc(sizeof(struct lfs_info));
    uint16_t buffer_size_count = 0;
    while (lfs_dir_read(fs, dir, info) > 0) {
        uint16_t name_buffer_size = u8_string_size(info->name) + 1;
        char *buffer = pvPortMalloc(name_buffer_size);
        memcpy(buffer, info->name, name_buffer_size);
        if (info->type == LFS_TYPE_DIR) {
            cc_slist_add_last(dir_list, buffer);
        } else {
            cc_slist_add_last(file_list, buffer);
        }
        buffer_size_count += name_buffer_size;
    }
    lfs_dir_close(fs, dir);
    vPortFree(info);
    cc_slist_sort(dir_list, _cc_slist_cmp_);
    cc_slist_sort(file_list, _cc_slist_cmp_);
    if (len < buffer_size_count + 1) {
        // can't fit, return false
        // clean up
        cc_slist_destroy_cb(dir_list, _cc_slist_free_u8str_);
        cc_slist_destroy_cb(file_list, _cc_slist_free_u8str_);
        return 0;
    }
    uint16_t buffer_offset = 0;
    CC_SListIter iter;
    char *next;
    cc_slist_iter_init(&iter, dir_list);
    while (cc_slist_iter_next(&iter, (void **)&next) == CC_OK) {
        uint16_t name_size = u8_string_size(next) + 1;
        memcpy((out_buffer + buffer_offset), next, name_size);
        buffer_offset += name_size;
        vPortFree(next);
        cc_slist_iter_replace(&iter, NULL, NULL);
    }
    cc_slist_destroy(dir_list);
    cc_slist_iter_init(&iter, file_list);
    while (cc_slist_iter_next(&iter, (void **)&next) == CC_OK) {
        uint16_t name_size = u8_string_size(next) + 1;
        memcpy((out_buffer + buffer_offset), next, name_size);
        buffer_offset += name_size;
        vPortFree(next);
        cc_slist_iter_replace(&iter, NULL, NULL);
    }
    cc_slist_destroy(file_list);
    out_buffer[buffer_size_count] = '\0'; // char \0
    return buffer_size_count + 1;
}
