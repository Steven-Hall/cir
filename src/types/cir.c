#include "../cir.h"
#include "list.h"
#include "../memory.h"

typedef struct cir {
    list* errors;
    list* functions;
} cir;

cir* cir_new(void) {
    cir* ir = xmalloc(sizeof(cir));
    ir -> errors = list_new(5, 5);
    ir -> functions = list_new(20, 10);
    return ir;
}

void cir_delete(cir* ir) {
    for(size_t i = 0; i < ir -> errors -> size; i++) {
        xfree(ir -> errors -> items[i]);
    }
    list_delete(ir -> errors);

    for(size_t i = 0; i < ir -> functions -> size; i++) {
        cir_function_delete(ir -> functions -> items[i]);
    }
    list_delete(ir -> functions);
    xfree(ir);
}

void cir_add_error(cir* ir, char* error) {
    list_append(ir -> errors, error);
}

size_t cir_error_count(cir* ir) {
    return ir -> errors -> size;
}

char* cir_get_error(cir* ir, size_t index) {
    if (index >= ir -> errors -> size) {
        return NULL;
    }
    return list_get(ir -> errors, index);
}

void cir_add_function(cir* ir, cir_function* function) {
    list_append(ir -> functions, function);
}

size_t cir_function_count(cir* ir) {
    return ir -> functions -> size;
}

cir_function* cir_get_function(cir* ir, size_t index) {
    if (index >= ir -> functions -> size) {
        return NULL;
    }
    return list_get(ir -> functions, index);
}
