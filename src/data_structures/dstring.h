#pragma once

#include "ddefines.h"



/*
    NOTE: This library is almost header only apart from one dependecy: `ddefines.h`. // TODO: Paste the defines stuff in here so you wont need to paste multible files.

    Using this header only library require you to do the following once and only once:
        #define DSTRING_IMPLEMENTATION
        #include "dstring.h"

        
        NOTE: If you implement one of the allocators functions on your own you need to implment them all
        
        
        Details:
        ` dstring is immutable use dstring_builder for dynamic operations like append, chop, ...
*/



#define DSTRING_NULL_TERMINATOR '\0'

typedef char * dstring;
typedef struct dstring_builder dstring_builder;

typedef struct dstring_memory_allocator
{
    void* (*allocate)(u64 size);
    void* (*reallocate)(void* block, u64 new_size);
    void  (*free)(void* block);
} dstring_memory_allocator;


dstring dstring___create__from_cstring(const char* cstr, dstring_memory_allocator* allocator); // Allocates dstring with size `strlen(cstr)` - from null-terminated cstr
dstring dstring___create__from_chars(const char chars[], u64 count, dstring_memory_allocator* allocator); // Allocates dstring with size `count` - from char[] chars
dstring dstring___create_copy__from_dstring(const dstring dstr, dstring_memory_allocator* allocator); // Allocates dstring with size `dstring___get__length(dstr)` - from dstring dstr
dstring dstring___create__from_cstring_format(dstring_memory_allocator* allocator, const char* format, ...); // Allocates dstring with size `snprintf(format, ...)` 
dstring dstring___concat__dstrings(const dstring first, const dstring second, dstring_memory_allocator* allocator); // Allocates dstring equals to copy of first concat with second
dstring dstring___concat__cstrings(const char *first, const char *second, dstring_memory_allocator* allocator); // Allocates dstring equals to copy of first concat with second, NOTE: both are null-terminated strings
dstring dstring___concat__chars(const char first[], const char second[], u64 first_count, u64 second_count, dstring_memory_allocator* allocator); // Allocates dstring equals to copy of first concat with second
dstring dstring___concat__by_delim__dstrings(const dstring first, const dstring second, const char join_delim, dstring_memory_allocator* allocator); // Allocates dstring equals to copy of first concat with join delim which then concat with second
dstring dstring___concat__by_delim__cstrings(const char* first, const char* second, const char join_delim, dstring_memory_allocator* allocator); // Allocates dstring equals to copy of first concat with join delim which then concat with second
dstring dstring___concat__by_delim__chars(const char first[], const char second[], u64 first_count, u64 second_count, const char join_delim, dstring_memory_allocator* allocator); // Allocates dstring equals to copy of first concat with join delim which then concat with second
dstring dstring___get__substring__by_delim(const dstring dstr, const char delim, bool from_start, dstring_memory_allocator* allocator); // return prefix of `dstr` to `delim` or suffix from `delim` based on from_start, NOTE return value does not contain the delim
dstring dstring___get__substring__by_delims(const dstring dstr, const char delims[], u64 delims_count, bool from_start, dstring_memory_allocator* allocator); // return prefix of `dstr` to `delims` or suffix from `delims` based on from_start, NOTE return value does not contain the delim
bool    dstring___is_contain__char(const dstring dstr, const char c, u64 * index); // Returns true if there exists index where `dstring[index] == c`, And if `index != NULL` then set index = first index where `dstring[index] == c`
bool    dstring___is_contain__dstring(const dstring text, const dstring pattern, u64* index); // Returns true if there exists a substring of text wich matches pattern, And if `index != NULL` then set index = first index of first match
bool    dstring___is_contain__cstring(const dstring text, const char* pattern, u64* index); // Returns true if there exists a substring of text wich matches pattern, And if `index != NULL` then set index = first index of first match, NOTE: cstring must be null-terminated
bool    dstring___is_prefix__dstring(const dstring text, const dstring pattern); // Returns true if text[0...pattern_length] == pattern
bool    dstring___is_prefix__cstring(const dstring text, const char* pattern); // Returns true if text[0...pattern_length] == pattern, NOTE: cstring must be null-terminated
bool    dstring___is_suffix__dstring(const dstring text, const dstring pattern); // Returns true if text[text_len - pattern_length - 1 ... text_length - 1] == pattern
bool    dstring___is_suffix__cstring(const dstring text, const char* pattern); // Returns true if text[text_len - pattern_length - 1 ... text_length - 1] == pattern, NOTE: cstring must be null-terminated
bool    dstring___compare__dstring(const dstring dstr1, const dstring dstr2); // return true <=> dstr1 == dstr2
bool    dstring___compare__cstring(const dstring dstr, const char* cstr); // return true <=> dstr1 == dstr2
void    dstring___destroy(dstring dstr, dstring_memory_allocator* allocator); // Free the allocation of `dstr`
void    dstring___convert__to_lower(dstring dstr); // Convert dstr uppercase to lowercase and all other characters dont change
void    dstring___convert__to_upper(dstring dstr); // Convert dstr lowercase to uppercase and all other characters dont change
void    dstring___replace__char(dstring dstr, char to_replace, char replacement); // replace the first `char == to_replace` in dstr to `replacement`
void    dstring___replace__chars(dstring dstr, char to_replace[], char replacement[], u64 length); // replace the first substring of dstr which equles to `to_replace` with `replacement`
u64     dstring___get__length(const dstring dstr); // Returns number of chars in dstring dstr

dstring_builder* dstring_builder___create(u64 initial_capacity, dstring_memory_allocator* allocator);
dstring          dstring_builder___generate__dstring(dstring_builder* builder, dstring_memory_allocator* allocator);
dstring          dstring_builder___chop__prefix__by_delim(dstring_builder* builder, const char delim, dstring_memory_allocator* allocator);
dstring          dstring_builder___chop__suffix__by_delim(dstring_builder* builder, const char delim, dstring_memory_allocator* allocator);
void             dstring_builder___destroy(dstring_builder* builder, dstring_memory_allocator* allocator);
void             dstring_builder___append__dstring(dstring_builder* builder, dstring dstr, dstring_memory_allocator* allocator);
void             dstring_builder___append__cstring(dstring_builder* builder, const char* cstring, dstring_memory_allocator* allocator);
void             dstring_builder___append__chars(dstring_builder* builder, const char chars[], u64 count, dstring_memory_allocator* allocator);
void             dstring_builder___append__char(dstring_builder* builder, const char c, dstring_memory_allocator* allocator);
void             dstring_builder___replace_at__dstring(dstring_builder* builder, dstring replacement ,u64 start_index);
void             dstring_builder___replace_at__cstring(dstring_builder* builder, const char* replacement ,u64 start_index);
void             dstring_builder___replace_at__chars(dstring_builder* builder, const char replacement[] , u64 count, u64 start_index);
void             dstring_builder___reserve(dstring_builder* builder, u64 amount, dstring_memory_allocator* allocator);
void             dstring_builder___remove_at__char(dstring_builder* builder, u64 index);
void             dstring_builder___remove_at__chars(dstring_builder* builder, u64 index, u64 count);
void             dstring_builder___remove_all__char(dstring_builder* builder, const char c);
void             dstring_builder___replace_all__char(dstring_builder* builder, const char to_replace, const char replacement);
void             dstring_builder___remove__prefix(dstring_builder * builder, u64 prefix_len);
void             dstring_builder___remove__suffix(dstring_builder * builder, u64 suffix_len);
u64              dstring_builder___get__capacity(dstring_builder* builder);
u64              dstring_builder___get__chars_count(dstring_builder* builder);

       
#if defined(DSTRING_IMPLEMENTATION) || 1

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#define dstrlen(dstr) (((u64*)dstr) - 1)[ 0 ];
#define char_to_lower(c) (char)('A' <= (c) && (c) <= 'Z') ? ((c) + 'a' - 'A') : (c) 
#define char_to_upper(c) (char)('a' <= (c) && (c) <= 'z') ? ((c) - 'a' + 'A') : (c) 

typedef struct dstring_builder {
    char * chars;
    u64 capacity, count;
} dstring_builder; 

#define DSTRING_ALLOCATE(size, allocator) allocator == NULL ? malloc(size) : allocator->allocate(size)
#define DSTRING_REALLOCATE(block, size, allocator) allocator == NULL ? realloc(block, size) : allocator->reallocate(block, size)
#define DSTRING_FREE(block, allocator) allocator == NULL ? free(block) : allocator->free(block)

dstring dstring___create__from_cstring(const char* cstr, dstring_memory_allocator* allocator)
{
    u64 cstr_len = strlen(cstr);
    
    u64* ptr = (u64* )DSTRING_ALLOCATE(sizeof(u64) + cstr_len + sizeof(char), allocator);
    ptr[ 0 ] = cstr_len;
    
    dstring dstr = (dstring)(ptr + 1);
    memcpy(dstr, cstr, cstr_len * sizeof(char));
    dstr[cstr_len] = DSTRING_NULL_TERMINATOR;

    return dstr;
}

dstring dstring___create__from_chars(const char chars[], u64 count, dstring_memory_allocator* allocator)
{
    u64* ptr = (u64* )DSTRING_ALLOCATE(sizeof(u64) + count + sizeof(char), allocator);
    ptr[ 0 ] = count;

    dstring dstr = (dstring)(ptr + 1);
    memcpy(dstr, chars, count * sizeof(char));
    dstr[count] = DSTRING_NULL_TERMINATOR;

    return dstr;
}

dstring dstring___create_copy__from_dstring(const dstring dstr, dstring_memory_allocator* allocator)
{
    u64* old_ptr = ((u64*)dstr) - 1;
    u64* new_ptr = (u64* )DSTRING_ALLOCATE(sizeof(u64) + old_ptr[0] + sizeof(char), allocator);
    dstring new_dstr = (dstring)(new_ptr + 1);

    memcpy(new_ptr, old_ptr, sizeof(u64) + old_ptr[0] + sizeof(char));

    return new_dstr;
}

dstring dstring___create__from_cstring_format(dstring_memory_allocator* allocator, const char *format, ...)
{
    va_list args;
    va_list args_copy;
    va_start(args, format);
    va_start(args_copy, format);

    u64 str_len = vsnprintf(NULL, 0, format, args_copy) + sizeof(char);

    u64* ptr = (u64* )DSTRING_ALLOCATE(sizeof(u64) + str_len + sizeof(char), allocator);
    dstring dstr = (dstring)(ptr + 1);
    
    ptr[ 0 ] = str_len;
    
    vsnprintf(dstr, str_len, format, args);
    dstr[str_len] = DSTRING_NULL_TERMINATOR;

    va_end(args);
    va_end(args_copy);

    return dstr;
}

dstring dstring___concat__dstrings(const dstring first, const dstring second, dstring_memory_allocator* allocator)
{
    u64 first_len  = dstrlen(first);
    u64 second_len = dstrlen(second);
    u64* ptr = (u64* )DSTRING_ALLOCATE(sizeof(u64) + first_len + second_len + sizeof(char), allocator);
    ptr[ 0 ] = first_len + second_len;

    dstring dstr = (dstring)(ptr + 1);

    memcpy(dstr, first, first_len * sizeof(char));
    memcpy(dstr + first_len, second, second_len * sizeof(char));

    dstr[first_len + second_len] = DSTRING_NULL_TERMINATOR;

    return dstr;
}

dstring dstring___concat__cstrings(const char *first, const char *second, dstring_memory_allocator* allocator)
{
    u64 first_len  = strlen(first);
    u64 second_len = strlen(second);
    u64* ptr = (u64* )DSTRING_ALLOCATE(sizeof(u64) + first_len + second_len + sizeof(char), allocator);
    ptr[ 0 ] = first_len + second_len;
    
    dstring dstr = (dstring)(ptr + 1);
    
    memcpy(dstr, first, first_len * sizeof(char));
    memcpy(dstr + first_len, second, second_len * sizeof(char));

    dstr[first_len + second_len] = DSTRING_NULL_TERMINATOR;
    
    return dstr;
}

dstring dstring___concat__chars(const char first[], const char second[], u64 first_count, u64 second_count, dstring_memory_allocator* allocator)
{
    u64* ptr = (u64* )DSTRING_ALLOCATE(sizeof(u64) + first_count + second_count + sizeof(char), allocator);
    ptr[ 0 ] = first_count + second_count;
    
    dstring dstr = (dstring)(ptr + 1);
    
    memcpy(dstr, first, first_count * sizeof(char));
    memcpy(dstr + first_count, second, second_count * sizeof(char));

    dstr[first_count + second_count] = DSTRING_NULL_TERMINATOR;
    
    return dstr;
}

dstring dstring___concat__by_delim__dstrings(dstring first, dstring second, char join_delim, dstring_memory_allocator* allocator)
{
    u64 first_len  = dstrlen(first);
    u64 second_len = dstrlen(second);
    u64* ptr = (u64* )DSTRING_ALLOCATE(sizeof(u64) + first_len + sizeof(char) + second_len + sizeof(char), allocator);
    ptr[ 0 ] = first_len + second_len + 1;

    dstring dstr = (dstring)(ptr + 1);

    memcpy(dstr, first, first_len * sizeof(char));
    dstr[first_len] = join_delim;
    memcpy(dstr + first_len + 1, second, second_len * sizeof(char));

    dstr[first_len + second_len + 1] = DSTRING_NULL_TERMINATOR;

    return dstr;    
}

dstring dstring___concat__by_delim__cstrings(const char *first, const char *second, const char join_delim, dstring_memory_allocator* allocator)
{
    u64 first_len  = strlen(first);
    u64 second_len = strlen(second);
    u64* ptr = (u64* )DSTRING_ALLOCATE(sizeof(u64) + first_len + sizeof(char) + second_len + sizeof(char), allocator);
    ptr[ 0 ] = first_len + second_len + 1;

    dstring dstr = (dstring)(ptr + 1);

    memcpy(dstr, first, first_len * sizeof(char));
    dstr[first_len] = join_delim;
    memcpy(dstr + first_len + 1, second, second_len * sizeof(char));

    dstr[first_len + second_len + 1] = DSTRING_NULL_TERMINATOR;

    return dstr; 
}

dstring dstring___concat__by_delim__chars(const char first[], const char second[], u64 first_count, u64 second_count, const char join_delim, dstring_memory_allocator* allocator)
{
    u64* ptr = (u64* )DSTRING_ALLOCATE(sizeof(u64) + first_count + sizeof(char) + second_count + sizeof(char), allocator);
    ptr[ 0 ] = first_count + second_count + 1;

    dstring dstr = (dstring)(ptr + 1);

    memcpy(dstr, first, first_count * sizeof(char));
    dstr[first_count] = join_delim;
    memcpy(dstr + first_count + 1, second, second_count * sizeof(char));

    dstr[first_count + second_count + 1] = DSTRING_NULL_TERMINATOR;

    return dstr; 
}

dstring dstring___get__substring__by_delim(const dstring dstr, const char delim, bool from_start, dstring_memory_allocator* allocator)
{
    u64 dstr_len = dstrlen(dstr);
    const char * ptr = dstr;
    if (from_start)
    {
        u64 index = 0;
        while (*ptr++ != delim && index++ < dstr_len);
        
        return dstring___create__from_chars(dstr, index, allocator);
    }
    else
    {
        u64 index = dstr_len - 1;
        while (*ptr++ != delim && index-- >= 0);

        return dstring___create__from_chars(dstr, index, allocator);
    }
}

dstring dstring___get__substring__by_delims(const dstring dstr, const char delims[], u64 delims_count, bool from_start, dstring_memory_allocator* allocator)
{
    u64 dstr_len = dstrlen(dstr);
    u64 start = from_start ? 0 : dstr_len - delims_count;
    u64 didx  = from_start ? 1 : -1;

    if (delims_count <= dstr_len)
    {

        for (u64 i = start; i >= 0 && i <= dstr_len - delims_count; i += didx)
        {
            bool match = true;
            for (u64 j = 0; j < delims_count; j++)
            {
                if (dstr[i + j] != delims[j])
                {
                    match = false;
                    break;
                }
            }
            
            if (match)
            {
                if (from_start)
                {
                    return dstring___create__from_chars(dstr, i, allocator);
                }
                else
                {
                    return dstring___create__from_chars(dstr + i + delims_count, dstr_len - i - delims_count, allocator);
                }
            }
        }
    }
    return dstring___create__from_cstring("", allocator); // empty string when error ????
}

inline u64 dstring___get__length(const dstring dstr)
{
    return (((u64*)dstr) - 1)[ 0 ];
}

bool dstring___is_contain__char(const dstring dstr, const char c, u64 *index)
{
    u64 dstr_len = (((u64*)dstr) - 1)[ 0 ]; // could use dstring___get__length(dstr);

    u64 idx = 0;
    for (char * at = dstr; at < dstr + dstr_len; at++, idx++)
    {
        if (at[0] == c)
        {
            if (index != NULL)
            {
                *index = idx;
                return true;
            }
        }
    }
    
    return false;
}

bool dstring___is_contain__dstring(const dstring text, const dstring pattern, u64 *index)
{
    u64 text_len = (((u64*)text) - 1)[ 0 ];
    u64 pattern_len = (((u64*)pattern) - 1)[ 0 ];

    if (pattern_len > text_len) return false;

    for (u64 i = 0; i < text_len - pattern_len; i++)
    {
        bool found = true;
        for (char * t = text + i, * p = pattern; p < pattern + pattern_len; p++, t++)
        {
            if (*t != *p)
            {
                found = false;
                break;
            }
        }

        if (found)
        {
            if (index != NULL)
            {
                *index = i;
            }
            return true;
        }
    }

    return false;
}

bool dstring___is_contain__cstring(const dstring text, const char *pattern, u64 *index)
{
    u64 text_len = (((u64*)text) - 1)[ 0 ];
    u64 pattern_len = strlen(pattern);

    if (pattern_len > text_len) return false;

    for (u64 i = 0; i < text_len - pattern_len; i++)
    {
        bool found = true;
        for (const char * t = text + i, * p = pattern; p < pattern + pattern_len; p++, t++)
        {
            if (*t != *p)
            {
                found = false;
                break;
            }
        }

        if (found)
        {
            if (index != NULL)
            {
                *index = i;
            }
            return true;
        }
    }

    return false;
}

bool dstring___is_prefix__dstring(const dstring text, const dstring pattern)
{
    u64 text_len = (((u64*)text) - 1)[ 0 ];
    u64 pattern_len = (((u64*)pattern) - 1)[ 0 ];

    if (pattern_len > text_len) return false;

    for (char * t = text, * p = pattern; p < pattern + pattern_len; p++, t++)
    {
        if (*t != *p)
        {
            return false;
        }
    }

    return true;
}

bool dstring___is_prefix__cstring(const dstring text, const char *pattern)
{
    u64 text_len = (((u64*)text) - 1)[ 0 ];
    u64 pattern_len = strlen(pattern);

    if (pattern_len > text_len) return false;

    for (const char * p = pattern , *t = text; p < pattern + pattern_len; p++, t++)
    {
        if (*t != *p)
        {
            return false;
        }
    }

    return true;
}

bool dstring___is_suffix__dstring(const dstring text, const dstring pattern)
{
    u64 text_len = (((u64*)text) - 1)[0];
    u64 pattern_len = (((u64*)pattern) - 1)[0];
    
    if (pattern_len >= text_len) return false;
    
    for (char* t = (text + text_len - pattern_len), *p = pattern; p < pattern + pattern_len; p++, t++)
    {
        if (*p != *t) return false;
    }
    
    return true;
}

bool dstring___is_suffix__cstring(const dstring text, const char *pattern)
{
    u64 text_len = (((u64*)text) - 1)[0];
    u64 pattern_len = strlen(pattern);
    
    if (pattern_len >= text_len) return false;
    const char* p = pattern;
    for (char* t = (text + text_len - pattern_len); p < pattern + pattern_len; p++, t++)
    {
        if (*p != *t) return false;
    }
    
    return true;
}

bool dstring___compare__dstring(const dstring dstr1, const dstring dstr2)
{
    u64 len1 = dstrlen(dstr1);
    u64 len2 = dstrlen(dstr2);

    if (len1 != len2) return false;

    const char * d1 = dstr1;
    const char * d2 = dstr2;

    for (u64 i = 0; i < len1; i++)
    {
        if (*d1++ != *d2++)
        {
            return false;
        }
    }

    return true;
}

bool dstring___compare__cstring(const dstring dstr, const char *cstr)
{
    u64 dstr_len = dstrlen(dstr);
    u64 cstr_len = strlen(cstr);

    if (dstr_len != cstr_len) return false;

    const char * d1 = dstr;
    const char * d2 = cstr;

    for (u64 i = 0; i < dstr_len; i++)
    {
        if (*d1++ != *d2++)
        {
            return false;
        }
    }

    return true;
}

inline void dstring___destroy(dstring dstr, dstring_memory_allocator* allocator)
{
    DSTRING_FREE((((u64*)dstr) - 1), allocator);
}

void dstring___convert__to_lower(dstring dstr)
{
    u64 dstr_len = dstrlen(dstr);
    
    for (char * d = dstr; d < dstr + dstr_len; d++)
    {
        *d = char_to_lower((*d));
    }
}

void dstring___convert__to_upper(dstring dstr)
{
    u64 dstr_len = dstrlen(dstr);
    
    for (char * d = dstr; d < dstr + dstr_len; d++)
    {
        *d = char_to_upper((*d));
    }
}

void dstring___replace__char(dstring dstr, char to_replace, char replacement)
{
    u64 dstr_len = dstrlen(dstr);
    
    for (char * d = dstr; d < dstr + dstr_len; d++)
    {
        if (*d == to_replace)
        {
            *d = replacement;
        } 
    }
}

void dstring___replace__chars(dstring dstr, char to_replace[], char replacement[], u64 length)
{
    u64 dstr_len = dstrlen(dstr);
    
    if (length >= dstr_len) return;

    for (u64 i = 0; i < dstr_len - length; i++)
    {
        bool match = true;

        for (u64 j = 0; j < length; j++)
        {
            if (dstr[i + j] != to_replace[j])
            {
                match = false;
                break;
            }
        }

        if (match)
        {
            for (u64 j = 0; j < length; j++)
            {
                dstr[i + j] = replacement[j];
            }
        }
    }
}

dstring_builder * dstring_builder___create(u64 initial_capacity, dstring_memory_allocator* allocator)
{
    dstring_builder * builder = DSTRING_ALLOCATE(sizeof(dstring_builder), allocator);

    builder->capacity = initial_capacity;
    builder->chars    = DSTRING_ALLOCATE(sizeof(char) * builder->capacity, allocator);
    builder->count    = 0;

    return builder;
}

inline dstring dstring_builder___generate__dstring(dstring_builder *builder, dstring_memory_allocator* allocator)
{
    return dstring___create__from_chars(builder->chars, builder->count, allocator);
}

dstring dstring_builder___chop__prefix__by_delim(dstring_builder *builder, const char delim, dstring_memory_allocator* allocator)
{
    u64 delim_index = 0;
    for (; delim_index < builder->count && builder->chars[delim_index] != delim; delim_index++);

    dstring prefix = dstring___create__from_chars(builder->chars, delim_index, allocator);

    dstring_builder___remove__prefix(builder, delim_index);
}

dstring dstring_builder___chop__suffix__by_delim(dstring_builder *builder, const char delim, dstring_memory_allocator* allocator)
{
    u64 delim_index = builder->count - 1;
    for (; delim_index >= 0 && builder->chars[delim_index] != delim; delim_index--);

    dstring dstr = dstring___create__from_chars(builder->chars + delim_index + 1, builder->count - delim_index, allocator);

    dstring_builder___remove__suffix(builder, builder->count - delim_index);

}

void dstring_builder___destroy(dstring_builder *builder, dstring_memory_allocator* allocator)
{
    if(builder != NULL)
    {
        if (builder->chars) DSTRING_FREE(builder->chars, allocator);
        DSTRING_FREE(builder, allocator);
    }
}

void dstring_builder___append__dstring(dstring_builder *builder, dstring dstr, dstring_memory_allocator* allocator)
{
    u64 dstr_len = dstrlen(dstr);
    if (builder->capacity <= builder->count + dstr_len)
    {
        builder->capacity += builder->count + dstr_len;
        builder->chars = DSTRING_REALLOCATE(builder->chars, builder->capacity, allocator); 
    }

    memcpy(builder->chars + builder->count, dstr, dstr_len);
    builder->count += dstr_len;
}

void dstring_builder___append__cstring(dstring_builder *builder, const char *cstring, dstring_memory_allocator* allocator)
{
    u64 dstr_len = strlen(cstring);
    if (builder->capacity <= builder->count + dstr_len)
    {
        builder->capacity += builder->count + dstr_len;
        builder->chars = DSTRING_REALLOCATE(builder->chars, builder->capacity, allocator); 
    }
    
    memcpy(builder->chars + builder->count, cstring, dstr_len);
    builder->count += dstr_len;
}

void dstring_builder___append__chars(dstring_builder *builder, const char chars[], u64 count, dstring_memory_allocator* allocator)
{
    if (builder->capacity <= builder->count + count)
    {
        builder->capacity += builder->count + count;
        builder->chars = DSTRING_REALLOCATE(builder->chars, builder->capacity, allocator); 
    }
    
    memcpy(builder->chars + builder->count, chars, count);
    builder->count += count;
}

void dstring_builder___append__char(dstring_builder *builder, const char c, dstring_memory_allocator* allocator)
{
    if (builder->capacity <= builder->count + 1)
    {
        builder->capacity *= 2;
        builder->chars = DSTRING_REALLOCATE(builder->chars, builder->capacity, allocator); 
    }
    
    builder->chars[builder->count++] = c;
}

void dstring_builder___replace_at__dstring(dstring_builder *builder, dstring replacement, u64 start_index)
{
    u64 replacement_len = dstrlen(replacement);
    memcpy(builder->chars + start_index, replacement, replacement_len);
    builder->count += replacement_len;
}

inline void dstring_builder___replace_at__cstring(dstring_builder *builder, const char *replacement, u64 start_index)
{
    u64 replacement_len = strlen(replacement);
    memcpy(builder->chars + start_index, replacement, replacement_len);
    builder->count += replacement_len;
}

inline void dstring_builder___replace_at__chars(dstring_builder *builder, const char replacement[], u64 count, u64 start_index)
{
    memcpy(builder->chars + start_index, replacement, count);
    builder->count += count;
}

inline u64 dstring_builder___get__capacity(dstring_builder *builder)
{
    return builder->capacity;
}

inline u64 dstring_builder___get__chars_count(dstring_builder *builder)
{
    return builder->count;
}

void dstring_builder___reserve(dstring_builder *builder, u64 amount, dstring_memory_allocator* allocator)
{
    builder->capacity += amount;
    builder->chars = DSTRING_REALLOCATE(builder->chars, builder->capacity, allocator);
}

inline void dstring_builder___remove_at__char(dstring_builder *builder, u64 index)
{
    memmove(builder->chars + index, builder->chars + index + 1, builder->count-- - index);
}

inline void dstring_builder___remove_at__chars(dstring_builder *builder, u64 index, u64 count)
{
    memmove(builder->chars + index, builder->chars + index + count, builder->count - index - count);
    builder->count -= count;
}

void dstring_builder___remove_all__char(dstring_builder *builder, const char c)
{
    char * ptr = builder->chars;
    int iteration = 0;
    int deleted_count = 0;
    
    while(iteration < builder->count)
    {
        if(*ptr == c)
        {
            memmove(ptr, ptr + 1, builder->count - deleted_count - (int)(ptr - builder->chars));
        }
        else
        {
            ptr++;
        }
        
        iteration++;
    }

    builder->count -= deleted_count;
}

void dstring_builder___replace_all__char(dstring_builder *builder, const char to_replace, const char replacement)
{
    for (char * p = builder->chars; p < builder->chars + builder->count; p++)
    {
        *p = *p == to_replace ? replacement : *p;
    }
}

inline void dstring_builder___remove__prefix(dstring_builder *builder, u64 prefix_len)
{
    memmove(builder->chars, builder->chars + prefix_len + 1, builder->count - prefix_len);
}

inline void dstring_builder___remove__suffix(dstring_builder *builder, u64 suffix_len)
{
    builder->count -= suffix_len;
}

#endif // defined(DSTRING_IMPLEMENTATION)