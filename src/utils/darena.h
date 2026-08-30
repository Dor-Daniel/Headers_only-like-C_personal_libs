#pragma once 
#include "ddefines.h"

typedef struct darena* darena_t;

darena_t darena_create(u64 init_capacity);
void     darena_destroy(darena_t arena);
void*    darena_get(darena_t arena, u64 size);
void     darena_release(darena_t arena, u64 size);

#if defined(DARENA_IMPLEMENTATION)

#include <stdlib.h>
#include <assert.h>
#include <string.h>

typedef struct darena {
    u64 offset;
    u64 capacity;
}* darena_t;

#define DARENA_BASE_POS(a) ((darena_t*)(a) + 1)
#define DARENA_MIN(a, b) a < b ? a : b

darena_t darena_create(u64 init_capacity)
{
    darena_t arena = (darena_t)malloc(sizeof(struct darena) + init_capacity);
    assert(arena);
    arena->offset = 0;
    arena->capacity = init_capacity;
    return arena;
}

void darena_destroy(darena_t arena)
{
    if (arena) free(arena);
}

void* darena_get(darena_t arena, u64 size)
{
    u64 new_cap = arena->capacity;
    while (new_cap < arena->offset + size)
    {
        new_cap *= 2;
    }
    if (new_cap > arena->capacity)
    {
        void * re = arena;
        re = realloc(re, arena->capacity * 2);
        assert(re);
        arena = re;
    }

    void* block = DARENA_BASE_POS(arena) + arena->offset;
    memset(block, 0, size);
    arena->offset += size;
    return block;
}

void darena_release(darena_t arena, u64 size)
{
    u64 shrink = DARENA_MIN(size, arena->offset);
    arena->offset -= shrink;
}


#endif