#pragma once
#include "../utils/ddefines.h"

typedef struct hash_tabel_mem_allocator {
    void* (*allocate)(u64);
    void* (*reallocate)(void*, u64);
    void (*free)(void*);
} dhash_tabel_mem_allocator;

typedef enum dhash_type_e {
    HASH_TYPE_MURMUR3_32, // https://en.wikipedia.org/wiki/MurmurHash#Algorithm
    HASH_TYPE_FNV1A, // https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function#FNV-1a_hash
    HASH_TYPE_AP, 
    HASH_TYPE_DEK, 
    HASH_TYPE_DJB, 
    HASH_TYPE_SDBM, 
    HASH_TYPE_COUNT
} dhash_type_e;

typedef struct dhash_table_t dhash_table_t;
typedef struct dhash_table_iter {
    dhash_table_t* table;
    u64 index;
} dhash_table_iter;


dhash_table_t* dhash_table_create(u64 capacity, u32 type_size_in_bytes, dhash_type_e hash_type, dhash_tabel_mem_allocator* allocator);
void dhash_table_destroy(dhash_table_t*table);
bool dhash_table_insert (dhash_table_t* table, const char* key, void* value);
bool dhash_table_remove (dhash_table_t* table, const char* key, void* out_val);
bool dhash_table_get    (dhash_table_t* table, const char* key, void* out_value);
bool dhash_iterator     (dhash_table_t* table, dhash_table_iter* out_iterator, void* out_first);
bool dhash_iterator_next(dhash_table_iter* out_iterator, void* out_next);

#if defined(DHASH_TABLE_IMPLEMENTATION)

#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define DEFAULT_HASH_TABLE_ALLOCATOR (dhash_tabel_mem_allocator){ .allocate = malloc, .reallocate = realloc, .free =  free }
#define BLOCK_FROM_TABLE(t) (u8*)((t) + 1)
#define TABLE_JUMP_TO(t, i) (u8*)(BLOCK_FROM_TABLE(t) + (i) * ( t->type_size + sizeof(bool) )) 

typedef struct dhash_table_t { 
    dhash_type_e type;
    u32 type_size;
    dhash_tabel_mem_allocator allocator;
    u64 capacity;
} dhash_table_t;


dhash_table_t* dhash_table_create(u64 capacity, u32 type_size_in_bytes, dhash_type_e hash_type, dhash_tabel_mem_allocator* _allocator)
{
    dhash_tabel_mem_allocator allocator = _allocator != NULL ? *_allocator : DEFAULT_HASH_TABLE_ALLOCATOR;
    dhash_table_t* table = allocator.allocate(sizeof(dhash_table_t) + capacity * (type_size_in_bytes + sizeof(bool)));
    assert(table);
    table->type = hash_type;
    table->capacity = capacity;
    table->allocator = allocator;
    table->type_size = type_size_in_bytes;
    memset(BLOCK_FROM_TABLE(table), 0, capacity * (type_size_in_bytes + sizeof(bool)));
    return table;
}

void dhash_table_destroy(dhash_table_t* table)
{
    if (table) table->allocator.free(table);
}

static u64 _hash_index_(dhash_type_e type ,const char* key)
{
    u64 h = 0;
    switch (type)
    {
        case HASH_TYPE_MURMUR3_32:
        {
            h = (u64)(key); // Seed ~ random by pointer
            u32 k;
            u32 keylen = strlen(key);
            const u8* ptr = (const u8*)key;
            for (u64 i = keylen >> 2; i > 0; i--)
            {
                memcpy(&k, ptr, sizeof(u32));
                ptr += sizeof(u32);
                u32 r = k;
                r *= 0xcc9e2d51;
                r = (r << 15) | (r >> 17);
                r *= 0x1b873593;
                h ^= r;
                h = (h << 13) | (h >> 19);
                h = h * 5 + 0xe6546b64; 
            }
            
            k = 0;
            for (u64 i = keylen & 3; i > 0; i--)
            {
                k <<= 8;
                k |= (u32)key[ keylen - i - 1 ];
            }
            
            u32 r = k;
            r *= 0xcc9e2d51;
            r = (r << 15) | (r >> 17);
            r *= 0x1b873593;
            h ^= r;
            h ^= (h >> 16);
            h *= 0x85ebca6b;
            h ^= (h >> 13);
            h *= 0xc2b2ae35;
            h ^= (h >> 16);
        } break;
        case HASH_TYPE_FNV1A:
        {
            const u64 prime = 1099511628211ULL;
            const u64 offset = 14695981039346656037ULL;
            h = offset;
            for (const u8* p = (const u8*)key; p < (u8*)key + strlen(key); p++)
            {
                h ^= (u64)(unsigned char)(*p);
                h *= prime;
            }
        } break;
        case HASH_TYPE_AP:
        {
            h = 0xAAAAAAAA;
            for (u64 i = 0; i < strlen(key); i++) {
                if ((i & 1) == 0) {
                    h ^= ((h << 7) ^ (u8)key[i] * (h >> 3));
                } else {
                    h ^= (~((h << 11) + ((u8)key[i] ^ (h >> 5))));
                }
            }
        } break;
        case HASH_TYPE_DEK:
        {
            h = strlen(key);
            u32 i;
            
            for (i = 0; i < strlen(key); ++i)
            {
                h = ((h << 5) ^ (h >> 27)) ^ (u8)(key[i]);
            }
        } break;
        case HASH_TYPE_DJB:
        {
            h = 5381;
            u32 i;
            
            for (i = 0; i < strlen(key); ++i)
            {
                h = ((h << 5) + h) + (u8)(key[i]);
            }
        } break;
        case HASH_TYPE_SDBM:
        {
            h = 0;
            u32 i;
            
            for (i = 0; i < strlen(key); ++i)
            {
                h = (u8)key[0] + (h << 6) + (h << 16) - h;
            }
        } break;
        default: break;
    }
    return h;
}

bool dhash_table_insert (dhash_table_t* table, const char* key, void* value)
{
    if (!table || !value) return false;
    
    u64 index = _hash_index_(table->type, key);
    index = index % table->capacity;
    bool* entry = (bool*)TABLE_JUMP_TO(table, index);
    if (!entry[0])
    {
        entry[0] = true;
        void* data = (void*)(entry + 1);
        memcpy(data, value, table->type_size);
        return true;
    }
    return false;
}

bool dhash_table_remove (dhash_table_t* table, const char* key, void* out_val)
{
    if (!table) return false;
    
    u64 index = _hash_index_(table->type, key);
    index = index % table->capacity;
    bool* entry = (bool*)TABLE_JUMP_TO(table, index);
    
    if (!entry[0]) return false;
    
    if (out_val)
    {
        memcpy(out_val, (void*)(entry + 1), table->type_size);
    }
    
    memset((void*)(entry + 1), 0, table->type_size);
    entry[0] = false;
    return true;
}

bool dhash_table_get (dhash_table_t* table, const char* key, void* out_value)
{
    if (!table || !out_value) return false;
    
    u64 index = _hash_index_(table->type, key);
    index = index % table->capacity;
    bool* entry = (bool*)TABLE_JUMP_TO(table, index);
    
    if (!entry[0]) return false;
    
    memcpy(out_value, (void*)(entry + 1), table->type_size);
    return true;
}

bool dhash_iterator(dhash_table_t *table, dhash_table_iter *out_iterator, void *out_first)
{
    if (!out_iterator) return false;
    out_iterator->table = table;
    bool* b = (bool*)BLOCK_FROM_TABLE(table);
    u64 index = 0;
    while (!*b && index < table->capacity)
    {
        index ++;
        b += (1 + table->type_size);
    }
    
    if (index == table->capacity) return false;
    else {
        if (out_first) memcpy(out_first, b + 1, table->type_size);
        out_iterator->index = index;
    }
    return true;
}

bool dhash_iterator_next(dhash_table_iter* out_iterator, void* out_next)
{
    if (!out_iterator) return false;
    if (out_iterator->index >= out_iterator->table->capacity) return false;
    bool* b = (bool*)TABLE_JUMP_TO(out_iterator->table, out_iterator->index);
    while (!*b && out_iterator->index < out_iterator->table->capacity)
    {
        out_iterator->index ++;
        b += (1 + out_iterator->table->type_size);
    }
    
    if (out_iterator->index == out_iterator->table->capacity) return false;
    else {
        if (out_next) memcpy(out_next, b + 1, out_iterator->table->type_size);
    }
    return true;
    
}

#endif