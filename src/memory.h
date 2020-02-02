#ifndef _CIR_MEMORY_H_
#define _CIR_MEMORY_H_

#include <stdlib.h>
#include <stdint.h>

void* xmalloc(uint64_t size);
void* xrealloc(void* ptr, uint64_t new_size);
void xfree(void* ptr);

#endif
