#ifndef _CIR_MEMORY_H_
#define _CIR_MEMORY_H_

#include <stdlib.h>
#include <stddef.h>

void* xmalloc(size_t size);
void* xrealloc(void* ptr, size_t new_size);
void xfree(void* ptr);

#endif
