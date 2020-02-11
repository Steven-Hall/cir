#include "memory.h"

void* xmalloc(size_t size) {
    return malloc(size);
}

void* xrealloc(void* ptr, size_t new_size) {
    return realloc(ptr, new_size);
}

void xfree(void* ptr) {
    free(ptr);
}
