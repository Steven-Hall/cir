#include "list.h"

list* list_new(uint64_t initial_capacity, uint64_t growth) {
    list* l = xmalloc(sizeof(list));
    l -> items = xmalloc(sizeof(void*) * initial_capacity);
    l -> size = 0;
    l -> capacity = initial_capacity;
    l -> growth = growth;
    return l;
}

void list_append(list* l, void* item) {
    if ((l -> size + 1) > l -> capacity) {
        l = xrealloc(l, (l -> capacity + l -> growth) * sizeof(void*));
        l -> capacity += l -> growth;
    }
    l -> items[l -> size] = item;
    l -> size++;
}

void list_delete(list* l) {
    xfree(l -> items);
    xfree(l);
}

void* list_get(list* l, uint64_t index) {
    if (index < l -> size) {
        return l -> items[index];
    }
    return NULL;
}
