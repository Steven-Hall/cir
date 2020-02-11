#ifndef _CIR_LIST_H_
#define _CIR_LIST_H_

#include <stdint.h>

#include "memory.h"

typedef struct list {
    void** items;
    size_t size;
    size_t capacity;
    size_t growth;
} list;

list* list_new(size_t initial_capacity, size_t growth);
void list_delete(list* list);
void list_append(list* list, void* item);
void* list_get(list* list, size_t item);

#endif
