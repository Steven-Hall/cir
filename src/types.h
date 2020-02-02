#ifndef _CIR_TYPES_H_
#define _CIR_TYPES_H_

#include <stdint.h>
#include <string.h>

#include "memory.h"
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

typedef struct cir_function_header {
    char* function_name;
} cir_function_header;

cir_function_header* cir_function_header_new(char* name);
void cir_function_header_delete(cir_function_header* h);

typedef struct cir_function_body {
    list* statements;
} cir_function_body;

cir_function_body* cir_function_body_new(void);
void cir_function_body_delete(cir_function_body* b);

typedef enum {
    S_ADD
} cir_statement_type;

typedef struct cir_statement {
    cir_statement_type type;
} cir_statement;
cir_statement* cir_statement_new(void);
void cir_statement_delete(cir_statement* s);

typedef struct cir_function {
    cir_function_header* header;
    cir_function_body* body;
} cir_function;

cir_function* cir_function_new(void);
void cir_function_delete(cir_function* f);
void cir_function_body_add_statement(cir_function_body* f, cir_statement* s);

typedef struct cir {
    list* functions;
    list* errors;
} cir;

cir* cir_new(void);
void cir_delete(cir* ir);

void cir_add_error(cir* ir, char* error);
uint64_t cir_error_count(cir* ir);
char* cir_get_error(cir* ir, uint64_t index);

void cir_add_function(cir* ir, cir_function* function);
uint64_t cir_function_count(cir* ir);
cir_function* cir_get_function(cir* ir, uint64_t index);

#endif
