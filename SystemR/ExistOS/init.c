
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/times.h>
#include <sys/types.h>
#include <unistd.h>
#include <malloc.h>

#include "timestamp.h"
#include "sys_llapi.h"
#include "FreeRTOS.h"
#include "task.h"
#include "SysConf.h"
#include "SysIRQ.h"
#include "debug.h"

extern unsigned int _sbss;
extern unsigned int _ebss;

extern void (*__preinit_array_start[])(void) __attribute__((weak));
extern void (*__preinit_array_end[])(void) __attribute__((weak));
extern void (*__init_array_start[])(void) __attribute__((weak));
extern void (*__init_array_end[])(void) __attribute__((weak));

bool g_system_in_emulator = false;
uint32_t g_system_symtab_hash = 0;

void main();

extern unsigned int __init_data;
extern unsigned int __data_start;
extern unsigned int __data_end;

const char __attribute__((section(".sysinfo"))) system_build_time[] = _TIMEZ_;//__DATE__ " " __TIME__;
extern uint32_t SYSTEM_STACK;

bool MemorySwapEnable = false;
uint32_t SwapMemorySize = 0;
uint32_t OnChipMemorySize = BASIC_RAM_SIZE;

void volatile _init_sys() __attribute__((section(".init"))) __attribute__((naked));
void volatile _init_sys() {

    __asm volatile(".word 0xEF5AE0EF");
    __asm volatile(".word 0xFECDAFDE");
    __asm volatile(".word 0x00000000");
    __asm volatile(".word 0x00000000");

    asm ("mov r13, %0" : : "r"((uint32_t)&SYSTEM_STACK));

    //set_r13((uint32_t)&SYSTEM_STACK);

    for (char *i = (char *)&_sbss; i < (char *)&_ebss; i++) {
        *i = 0; // clear bss
    }

    uint32_t *pui32Src, *pui32Dest;
    pui32Src = (uint32_t *)&__init_data;
    for (pui32Dest =  (uint32_t *)&__data_start; pui32Dest <  (uint32_t *)&__data_end;) {
        *pui32Dest++ = *pui32Src++;
    }



    //__libc_init_array();
    typedef void (*pfunc)();
    extern pfunc __ctors_start__[];
    extern pfunc __ctors_end__[];
    pfunc *p;
    for (p = __ctors_start__; p < __ctors_end__; p++) {
        (*p)();
    }

    g_system_in_emulator = ((uint32_t *)(_init_sys))[2];
    g_system_symtab_hash = ((uint32_t *)(_init_sys))[3];

    // init memory
    uint32_t free, total, total_comp;
    total_comp = ll_mem_phy_info(&free, &total);
    if (total_comp > OnChipMemorySize) {
        OnChipMemorySize = total_comp;
    }
    SwapMemorySize = ll_mem_swap_size();

    // init IRQ
    ll_set_irq_stack((uint32_t)&SYSTEM_STACK);
    ll_set_irq_vector(((uint32_t)IRQ_ISR) + 4);
    ll_set_svc_stack(((uint32_t)&SYSTEM_STACK) - 0x500);
    ll_set_svc_vector(((uint32_t)SWI_ISR) + 4);
    ll_enable_irq(false);

    main();

    while (1)
        ;
}

extern void (*__fini_array_start[])(void);
extern void (*__fini_array_end[])(void);

uint32_t __attribute__((naked)) getCurStackAdr() {
    __asm volatile("mov r0,r13");
    __asm volatile("mov pc,lr");
}

void __libc_fini_array(void) {
    size_t count;
    size_t i;

    count = __fini_array_end - __fini_array_start;
    for (i = count; i > 0; i--)
        __fini_array_start[i - 1]();
}

#undef errno
extern int errno;

extern char __HEAP_START asm("__HEAP_START");

void __sync_synchronize() {
}

static void *heap = NULL;
static int cur_heap_loc = 0;
caddr_t _sbrk(uint32_t incr) {

    char *prev_heap;
    if (heap == NULL) {
        heap = &__HEAP_START;
    }
    prev_heap = heap;

    if (cur_heap_loc == 0) {
        if (((uint32_t)heap + incr) > RAM_BASE + OnChipMemorySize) {
            if (MemorySwapEnable) {
                heap = (void *)(RAM_BASE + OnChipMemorySize);
                cur_heap_loc = 1;
                prev_heap = heap;
                heap += incr;
                ll_put_str("EXT HEAP 2\n");
                goto success;
            } else {

                ll_put_str("EXT HEAP NOMEM !\n");
                errno = ENOMEM;
                goto fail;
            }
        } else {
            heap += incr;
        }
    } else if (cur_heap_loc == 1) {

        if (((uint32_t)heap + incr) > RAM_BASE + OnChipMemorySize + SwapMemorySize) {
            ll_put_str("SWAP HEAP NOMEM !\n");
            errno = ENOMEM;
            goto fail;
        } else {
            heap += incr;
        }
    }
success:
    return (caddr_t)prev_heap;
fail:

    return (caddr_t)-1;
}

size_t getOnChipHeapAllocated() {
    if ((uint32_t)heap > RAM_BASE + OnChipMemorySize) {
        return OnChipMemorySize;
    } else {
        return (uint32_t)heap - ((uint32_t)&__HEAP_START);
    }
}

size_t getSwapMemHeapAllocated() {
    if ((uint32_t)heap > RAM_BASE + OnChipMemorySize) {
        return (uint32_t)heap - RAM_BASE - OnChipMemorySize;

    } else {
        return 0;
    }
}

uint32_t getHeapAllocateSize() {
    struct mallinfo info = mallinfo();
    return info.uordblks;
}

uint32_t getHeapAddr() {
    return (uint32_t)heap;
}

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName) {
    PANIC("SYS StackOverflowHook:%s\n", pcTaskName);
}

void vAssertCalled(char *file, int line) {
    PANIC("SYS ASSERTION FAILED AT %s:%d\n", file, line);
}

void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
                                    StackType_t **ppxTimerTaskStackBuffer,
                                    uint32_t *pulTimerTaskStackSize) {
    *ppxTimerTaskTCBBuffer = (StaticTask_t *)pvPortMalloc(sizeof(StaticTask_t));
    *ppxTimerTaskStackBuffer = (StackType_t *)pvPortMalloc(configMINIMAL_STACK_SIZE * 4);
    *pulTimerTaskStackSize = configMINIMAL_STACK_SIZE;
}

void vApplicationMallocFailedHook() {
    PANIC("SYS ASSERT: Out of Memory.\n");
}

void vApplicationIdleHook(void) {
    ll_system_idle();
}

int _close_r(struct _reent *pReent, int fd) {
    pReent->_errno = ENOTSUP;
    return -1;
}

int _execve_r(struct _reent *pReent, const char *filename, char *const *argv, char *const *envp) {
    pReent->_errno = ENOTSUP;
    return -1;
}

int _fork_r(struct _reent *pReent) {
    pReent->_errno = ENOTSUP;
    return -1;
}

int _getpid_r(struct _reent *pReent) {
    pReent->_errno = ENOTSUP;
    return -1;
}

int _isatty_r(struct _reent *pReent, int file) {
    pReent->_errno = ENOTSUP;
    return -1;
}

int _isatty(int file) {
    return -1;
}

int _kill_r(struct _reent *pReent, int pid, int signal) {
    pReent->_errno = ENOTSUP;
    return -1;
}

_CLOCK_T_ _times_r(struct _reent *pReent, struct tms *tbuf) {
    pReent->_errno = ENOTSUP;
    // ll_putStr("GET TIME\n");
    return -1;
}

int _wait_r(struct _reent *pReent, int *wstat) {
    pReent->_errno = ENOTSUP;
    return -1;
}

int _gettimeofday_r(struct _reent *pReent, struct timeval *__tp, void *__tzp) {
    pReent->_errno = ENOTSUP;

    printf("GET _gettimeofday_r\n");
    return -1;
}

char *getcwd(char *buf, size_t size) {

    return NULL;
}

void abort(void) {
    // Abort called
    printf("abort\n");
    while (1)
        ;
}

void _exit(int i) {
    printf("_exit\n");
    while (1)
        ;
}

void *__wrap_malloc(size_t sz) {
    void *mem;
    vTaskSuspendAll();

    // ll_put_str("malloc\n");

    mem = _malloc_r(_impure_ptr, sz);
    (void)xTaskResumeAll();

    return mem;
}

void __wrap_free(void *m) {
    // ll_put_str("free\n");

    vTaskSuspendAll();

    _free_r(_impure_ptr, m);

    (void)xTaskResumeAll();
}