#include "memory.h"

void* xmalloc(uint64_t size) {
    return malloc(size);
}

void* xrealloc(void* ptr, uint64_t new_size) {
    return realloc(ptr, new_size);
}

void xfree(void* ptr) {
    free(ptr);
}
