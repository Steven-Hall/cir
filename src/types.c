#include "types.h"

cir_atom* integer_atom_new(uint64_t value) {
    cir_atom* a = xmalloc(sizeof(cir_atom));
    a -> integer = value;
    return a;
}

cir_atom* identifier_atom_new(identifier value) {
    uint64_t value_length = strlen(value);
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

cir_function_header* cir_function_header_new(char* name) {
    cir_function_header* h = xmalloc(sizeof(cir_function_header));
    h -> function_name = name;
    return h;
}

void cir_function_header_delete(cir_function_header* h) {
    if (h == NULL) { return; }
    xfree(h -> function_name);
    xfree(h);
}

cir_atom* cir_identifier_atom_new(char* identifier) {
    cir_atom* a = malloc(sizeof(cir_atom));
    a -> type = ATOM_IDENTIFIER;
    a -> identifier = identifier;
    return a;
}

cir_atom* cir_integer_atom_new(uint64_t integer) {
    cir_atom* a = malloc(sizeof(cir_atom));
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

cir_statement* cir_return_statement_new(void) {
    cir_statement* s = malloc(sizeof(cir_statement));
    s -> type = S_RETURN;
    return s;
}

cir_statement* cir_move_statement_new(char* destination, cir_atom* source) {
    cir_statement* s = malloc(sizeof(cir_statement));
    s -> type = S_MOVE;
    s -> m.destination = destination;
    s -> m.source = source;
    return s;
}

cir_statement* cir_label_statement_new(char* label_name, cir_function_body* children) {
    cir_statement* s = malloc(sizeof(cir_statement));
    s -> type = S_LABEL;
    s -> l.name = label_name;
    s -> l.children = children;
    return s;
}

cir_statement* cir_jump_statement_new(char* label) {
    cir_statement* s = malloc(sizeof(cir_statement));
    s -> type = S_JUMP;
    s -> j.label = label;
    return s;
}

cir_statement* cir_bin_operator_statement_new(char* dst, char* left, char* right, cir_operator type) {
    cir_statement* s = malloc(sizeof(cir_statement));
    s -> type = S_BIN_OP;
    s -> o.dst = dst;
    s -> o.left = left;
    s -> o.right = right;
    s -> o.operator = type;
    return s;
}

cir_statement* cir_if_statement_new(char* condition, cir_function_body* true_path, cir_function_body* false_path) {
    cir_statement* s = malloc(sizeof(cir_statement));
    s -> type = S_IF;
    s -> i.condition = condition;
    s -> i.true_path = true_path;
    s -> i.false_path = false_path;
    return s;
}

void cir_statement_delete(cir_statement* s) {
    switch(s -> type) {
        case S_MOVE:
            free(s -> m.destination);
            free(s -> m.source);
        case S_LABEL:
            free(s -> l.name);
            cir_function_body_delete(s -> l.children);
        case S_JUMP:
            free(s -> j.label);
        case S_IF:
            free(s -> i.condition);
            cir_function_body_delete(s -> i.true_path);
            cir_function_body_delete(s -> i.false_path);
        case S_BIN_OP:
            free(s -> o.dst);
            free(s -> o.left);
            free(s -> o.right);
        default:
            free(s);
    }
}

cir_function_body* cir_function_body_new(void) {
    cir_function_body* b = xmalloc(sizeof(cir_function_body));
    b -> statements = list_new(10, 5);
    return b;
}

void cir_function_body_delete(cir_function_body* b) {
    if (b == NULL) { return; }
    for (uint64_t i = 0; i < b -> statements -> size; i++) {
        cir_statement_delete(b -> statements -> items[i]);
    }
    list_delete(b -> statements);
    xfree(b);
}

void cir_function_body_add_statement(cir_function_body* b, cir_statement* s) {
    list_append(b -> statements, s);
}

cir_function* cir_function_new(void) {
    cir_function* f = xmalloc(sizeof(cir_function));
    f -> header = NULL;
    f -> body = NULL;
    return f;
}

void cir_function_delete(cir_function* f) {
    cir_function_header_delete(f -> header);
    cir_function_body_delete(f -> body);
    xfree(f);
}

cir* cir_new(void) {
    cir* ir = xmalloc(sizeof(cir));
    ir -> errors = list_new(5, 5);
    ir -> functions = list_new(20, 10);
    return ir;
}

void cir_delete(cir* ir) {
    for(uint64_t i = 0; i < ir -> errors -> size; i++) {
        xfree(ir -> errors -> items[i]);
    }
    list_delete(ir -> errors);

    for(uint64_t i = 0; i < ir -> functions -> size; i++) {
        cir_function_delete(ir -> functions -> items[i]);
    }
    list_delete(ir -> functions);
    xfree(ir);
}

void cir_add_error(cir* ir, char* error) {
    list_append(ir -> errors, error);
}

uint64_t cir_error_count(cir* ir) {
    return ir -> errors -> size;
}

char* cir_get_error(cir* ir, uint64_t index) {
    if (index >= ir -> errors -> size) {
        return NULL;
    }
    return list_get(ir -> errors, index);
}

void cir_add_function(cir* ir, cir_function* function) {
    list_append(ir -> functions, function);
}

uint64_t cir_function_count(cir* ir) {
    return ir -> functions -> size;
}

cir_function* cir_get_function(cir* ir, uint64_t index) {
    if (index >= ir -> functions -> size) {
        return NULL;
    }
    return list_get(ir -> functions, index);
}

