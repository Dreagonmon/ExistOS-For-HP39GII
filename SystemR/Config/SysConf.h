#ifndef __SYSCONF_H__
#define __SYSCONF_H__

#define FS_FATFS        0
#define FS_LITTLEFS     1
#define FS_TYPE         FS_FATFS

#define RAM_BASE            (0x02000000)
#define BASIC_RAM_SIZE      (160 * 1024)
#define BASIC_HEAP_END      (RAM_BASE + BASIC_RAM_SIZE)


/*
malloc() priority:
   high:  Basic Memory   (~400 KB)
   low:   Swap Memory    (2~3MB, default disable)

======================
|    Swap Memory     | 
|  (Swap with flash) |
|      (2 MB)        | (default disable)
|                    | 
|---BASIC RAM END----|
|    Extern HEAP     | 
|      ~240KB        | (Enable memory compression and assuming the compression ratio of 0.3 ~ 0.4)
|                    |  
|   Basic HEAP       | (KhiCAS and other app stack default allocated from here)
|    (~160 K)        | 
|--------------------|
|  .sys_stack        |
|  .bss .data ...    |  
=======RAM_BASE======= (0x02000000)
   
*/

#endif
