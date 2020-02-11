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
        uintmax_t integer;
    };
} cir_atom;

cir_atom* cir_identifier_atom_new(char* identifier);
cir_atom* cir_integer_atom_new(uintmax_t integer);
void cir_atom_delete(cir_atom* a);

typedef struct cir_function_header {
    char* function_name;
} cir_function_header;

cir_function_header* cir_function_header_new(char* name);
void cir_function_header_delete(cir_function_header* h);

struct cir_statement; // defined below
typedef struct list cir_statement_list;

cir_statement_list* cir_statement_list_new(uintmax_t capacity, uintmax_t growth);
void cir_statement_list_delete(cir_statement_list* l);

typedef enum {
    S_RETURN,
    S_MOVE,
    S_LABEL,
    S_JUMP,
    S_IF,
    S_BIN_OP,
} cir_statement_type;

typedef enum {
    S_MOD,
    S_OR,
    S_GT,
    S_ADD,
} cir_operator;

typedef struct cir_move_statement {
    char* destination;
    cir_atom* source;
} cir_move_statement;

typedef struct cir_label_statement {
    char* name;
    cir_statement_list* statements;
} cir_label_statement;


typedef struct cir_jump_statement {
    char* label;
} cir_jump_statement;

typedef struct cir_if_statement {
    char* condition;
    cir_statement_list* true_path;
    cir_statement_list* false_path;
} cir_if_statement;

typedef struct cir_bin_op_statement {
    char* dst;
    char* left;
    char* right;
    cir_operator operator;
} cir_bin_op_statement;

typedef struct cir_statement {
    cir_statement_type type;
    union {
        cir_move_statement m;
        cir_label_statement l;
        cir_jump_statement j;
        cir_if_statement i;
        cir_bin_op_statement o;
    };
} cir_statement;

cir_statement* cir_return_statement_new(void);
cir_statement* cir_move_statement_new(char* destination, cir_atom* source);
cir_statement* cir_label_statement_new(char* label_name, cir_statement_list* children);
cir_statement* cir_jump_statement_new(char* label);
cir_statement* cir_if_statement_new(char* condition, cir_statement_list* true_path, cir_statement_list* false_path);
cir_statement* cir_bin_operator_statement_new(char* dst, char* left, char* right, cir_operator type);
void cir_statement_delete(cir_statement* s);

typedef struct cir_function {
    cir_function_header* header;
    cir_statement_list* statements;
} cir_function;

cir_function* cir_function_new(void);
void cir_function_delete(cir_function* f);
void cir_statement_list_add(cir_statement_list* l, cir_statement* s);

typedef struct cir {
    list* functions;
    list* errors;
} cir;

cir* cir_new(void);
void cir_delete(cir* ir);

void cir_add_error(cir* ir, char* error);
size_t cir_error_count(cir* ir);
char* cir_get_error(cir* ir, size_t index);

void cir_add_function(cir* ir, cir_function* function);
size_t cir_function_count(cir* ir);
cir_function* cir_get_function(cir* ir, size_t index);

#endif
