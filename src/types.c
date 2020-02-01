#include "types.h"

cir_atom* integer_atom_new(uint64_t value) {
    cir_atom* a = malloc(sizeof(cir_atom));
    a -> integer = value;
    return a;
}

cir_atom* identifier_atom_new(identifier value) {
    uint64_t value_length = strlen(value);
    cir_atom* a = malloc(sizeof(cir_atom));
    a -> identifier = malloc(sizeof(char) * value_length + 1);
    strncpy(a -> identifier, value, value_length);
    a -> identifier[value_length] = 0;
    return a;
}

void atom_delete(cir_atom* a) {
    if (a -> type == ATOM_IDENTIFIER) {
        free(a -> identifier);
    }
    free(a);
}

cir* cir_new(void) {
    cir* ir = malloc(sizeof(cir));
    return ir;
}

void cir_delete(cir* ir) {
    free(ir);
}
