#include "list.h"

list* list_new(uint64_t initial_capacity, uint64_t growth) {
    list* l = malloc(sizeof(list));
    l -> items = malloc(sizeof(void*) * initial_capacity);
    l -> size = 0;
    l -> capacity = initial_capacity;
    l -> growth = growth;
    return l;
}

void list_append(list* l, void* item) {
    if ((l -> size + 1) > l -> capacity) {
        // TODO dangerous to realloc like this as memory leaks if it fails
        l = realloc(l, (l -> capacity + l -> growth) * sizeof(void*));
        l -> capacity += l -> growth;
    }
    l -> items[l -> size] = item;
    l -> size++;
}

void list_delete(list* l) {
    free(l -> items);
    free(l);
}

void* list_get(list* l, uint64_t index) {
    if (index < l -> size) {
        return l -> items[index];
    }
    return NULL;
}
