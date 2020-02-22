#include "../../include/cir.h"

#include <string.h>
#include "../memory.h"

typedef char* identifier;

typedef struct cir_atom {
    cir_atom_type type;
    union {
        char* identifier;
        intmax_t integer;
    };
} cir_atom;

cir_atom* integer_atom_new(uintmax_t value) {
    cir_atom* a = xmalloc(sizeof(cir_atom));
    a -> integer = value;
    return a;
}

cir_atom* identifier_atom_new(identifier value) {
    size_t value_length = strlen(value);
    cir_atom* a = xmalloc(sizeof(cir_atom));
    a -> identifier = xmalloc(sizeof(char) * value_length + 1);
    strncpy(a -> identifier, value, value_length);
    a -> identifier[value_length] = 0;
    return a;
}

void atom_delete(cir_atom* a) {
    if (a -> type == ATOM_IDENTIFIER) {
        xfree(a -> identifier);
    }
    xfree(a);
}

cir_atom* cir_identifier_atom_new(char* identifier) {
    cir_atom* a = xmalloc(sizeof(cir_atom));
    a -> type = ATOM_IDENTIFIER;
    a -> identifier = identifier;
    return a;
}

cir_atom* cir_integer_atom_new(uintmax_t integer) {
    cir_atom* a = xmalloc(sizeof(cir_atom));
    a -> type = ATOM_INTEGER;
    a -> integer = integer;
    return a;
}

void cir_atom_delete(cir_atom* a) {
    if (a -> type == ATOM_IDENTIFIER) {
        xfree(a -> identifier);
    }
    xfree(a);
}

char* cir_atom_string_value(cir_atom* a) {
    if (a -> type == ATOM_IDENTIFIER) {
        return a -> identifier;
    }
    char* value = xmalloc(sizeof(intmax_t) * 21);
    snprintf(value, 20, "%ld", a -> integer);
    value[20] = 0;
    return value;
}
