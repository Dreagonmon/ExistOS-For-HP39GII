#pragma once

#include <stdio.h>
#include <stdint.h>

#define PANIC(...)          \
    do {                     \
        printf(__VA_ARGS__); \
        while (1)            \
            ;                \
    } while (0)

#define INFO(...)            \
    do {                     \
        printf(__VA_ARGS__); \
    } while (0)


uint32_t getHeapAllocateSize(void);

#define MEM_USED(label) printf("MEM %s: %ld\n", label, getHeapAllocateSize())
#define MEM_MALLOC(p) printf("MEM Alloc: %p\n", p)
#define MEM_FREE(p) printf("MEM Free: %p\n", p)
