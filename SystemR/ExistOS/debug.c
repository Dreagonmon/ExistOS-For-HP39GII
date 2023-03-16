#include <malloc.h>
#include "debug.h"

uint32_t getHeapAllocateSize() {
    struct mallinfo info = mallinfo();
    return info.uordblks;
}
