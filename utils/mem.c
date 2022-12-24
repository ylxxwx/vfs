#include "type.h"
#include "mem.h"
#include "stdlib.h"
#include "string.h"

u8* alloc_mem(u32 size) {
    return malloc(size);
}

void free_mem(u8* buffer) {
    free(buffer);
}

void clear_mem(u8* mem, u8 val, u32 size) {
    memset(mem, val, size);
}

char* move_str(u8* dst, u8 *src, u32 len) {
    return strncpy(dst, src, len);
}

void mov_mem(u8* dst, u8 *src, u32 len) {
    memcpy(dst, src, len);
}

char cmp_str(u8* dst, u8* src, u32 len) {
    return strncmp(dst, src, len);
}