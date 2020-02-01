#ifndef _CIR_LIST_H_
#define _CIR_LIST_H_

#include <stdint.h>
#include <stdlib.h>

typedef struct list {
    void** items;
    uint64_t size;
    uint64_t capacity;
    uint64_t growth;
} list;

list* list_new(uint64_t initial_capacity, uint64_t growth);
void list_delete(list* list);
void list_append(list* list, void* item);
void* list_get(list* list, uint64_t item);

#endif
