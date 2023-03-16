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
#define MEM_ACTION_MALLOC(p) printf("MEM ACTION Alloc: %p\n", p)
#define MEM_ACTION_FREE(p) printf("MEM ACTION Free: %p\n", p)
