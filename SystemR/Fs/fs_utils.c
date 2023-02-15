#include "fs_utils.h"
#include "ff.h"
#include "FreeRTOS.h"

static FATFS fs;

uint8_t mount_file_system() {
    FRESULT fres;
    fres = f_mount(&fs, FS_FLASH_PATH, 1);
    return fres == FR_OK;
}

uint8_t format_file_system() {
    FRESULT fres;
    BYTE *work = (BYTE *)pvPortMalloc(FF_MAX_SS);
    fres = f_mkfs(FS_FLASH_PATH, NULL, work, FF_MAX_SS); // use default format params: NULL
    vPortFree(work);
    return fres == FR_OK;
}
