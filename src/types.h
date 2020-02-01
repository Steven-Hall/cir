#ifndef _CIR_TYPES_H_
#define _CIR_TYPES_H_

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"

typedef enum {
    ATOM_IDENTIFIER,
    ATOM_INTEGER,
} cir_atom_type;

typedef char* identifier;

typedef struct cir_atom {
    cir_atom_type type;
    union {
        identifier identifier;
        uint64_t integer;
    };
} cir_atom;

typedef struct cir {
    list* functions;
    list* errors;
} cir;

cir* cir_new(void);
void cir_delete(cir* ir);

#endif
