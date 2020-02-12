#ifndef _CIR_H_
#define _CIR_H_

#include <stddef.h>
#include <stdint.h>

typedef struct cir cir;

const cir* cir_from_string(const char* source);
const cir* cir_from_file(const char* filename);
const char* cir_to_string(cir* ir);
void cir_to_file(const cir* ir, const char* filename);


typedef struct cir_function cir_function;
typedef struct cir_function_header cir_function_header;

typedef enum {
    ATOM_INTEGER,
    ATOM_IDENTIFIER,
} cir_atom_type;

typedef struct cir_atom cir_atom;

typedef struct cir_statement cir_statement;
typedef struct list cir_statement_list;
typedef struct cir_if_statement cir_if_statement;
typedef struct cir_return_statement cir_return_statement;

typedef enum {
    S_RETURN,
    S_MOVE,
    S_LABEL,
    S_JUMP,
    S_IF,
    S_BIN_OP,
} cir_statement_type;

typedef enum {
    S_GT,
    S_OR,
    S_ADD,
    S_MOD,
} cir_operator;

// cir
cir* cir_new(void);
void cir_delete(cir* cir);
void cir_add_error(cir* ir, char* error);
size_t cir_error_count(cir* ir);
char* cir_get_error(cir* ir, size_t index);
void cir_add_function(cir* ir, cir_function* function);
size_t cir_function_count(cir* ir);
cir_function* cir_get_function(cir* ir, size_t index);

// cir_function_header
cir_function_header* cir_function_header_new(char* function_name);
void cir_function_header_delete(cir_function_header* h);

// cir_function
cir_function* cir_function_new(cir_function_header* header, cir_statement_list* body);
void cir_function_delete(cir_function* f);
void cir_statement_list_add(cir_statement_list* l, cir_statement* s);

// cir_statement_list
cir_statement_list* cir_statement_list_new(size_t capacity, size_t growth);
void cir_statement_list_delete(cir_statement_list* l);

// cir statements
cir_statement* cir_if_statement_new(char* condition, cir_statement_list* true_statements, cir_statement_list* false_statements);
cir_statement* cir_return_statement_new(void);
cir_statement* cir_move_statement_new(char* dst, cir_atom* src);
cir_statement* cir_bin_operator_statement_new(char* dst, char* left, char* right, cir_operator op);
cir_statement* cir_label_statement_new(char* label, cir_statement_list* statements);
cir_statement* cir_jump_statement_new(char* label);

void cir_statement_delete(cir_statement* s);

// cir atoms
cir_atom* cir_integer_atom_new(uintmax_t value);
cir_atom* cir_identifier_atom_new(char* value);

void cir_atom_delete(cir_atom* a);

#endif