#include "sys_llapi.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "fs_utils.h"

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
