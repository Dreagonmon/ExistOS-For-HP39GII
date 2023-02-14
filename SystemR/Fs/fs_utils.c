#include "fs_utils.h"
#include "ff.h"
#include "FreeRTOS.h"

void initFS() {
    FRESULT fres;
    FATFS *fs = (FATFS *)pvPortMalloc(sizeof(FATFS));
    uint32_t keys = ll_vm_check_key() & 0xFFFF;

    fres = f_mount(fs, FS_FLASH_PATH, 1);
    if (fres != FR_OK) {
        BYTE *work = (BYTE *)pvPortMalloc(FF_MAX_SS);
        fres = f_mkfs(FS_FLASH_PATH, 0, work, FF_MAX_SS);
        // printf("mkfs:%d\n", fres);
        vPortFree(work);
    }
}
