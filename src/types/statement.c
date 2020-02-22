#include "../../include/cir.h"

#include "../memory.h"
#include "list.h"

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

cir_statement* cir_return_statement_new(void) {
    cir_statement* s = xmalloc(sizeof(cir_statement));
    s -> type = S_RETURN;
    return s;
}

cir_statement* cir_move_statement_new(char* destination, cir_atom* source) {
    cir_statement* s = xmalloc(sizeof(cir_statement));
    s -> type = S_MOVE;
    s -> m.destination = destination;
    s -> m.source = source;
    return s;
}

cir_statement* cir_label_statement_new(char* label_name, cir_statement_list* statements) {
    cir_statement* s = xmalloc(sizeof(cir_statement));
    s -> type = S_LABEL;
    s -> l.name = label_name;
    s -> l.statements = statements;
    return s;
}

cir_statement* cir_jump_statement_new(char* label) {
    cir_statement* s = xmalloc(sizeof(cir_statement));
    s -> type = S_JUMP;
    s -> j.label = label;
    return s;
}

cir_statement* cir_binop_statement_new(char* dst, char* left, char* right, cir_operator type) {
    cir_statement* s = xmalloc(sizeof(cir_statement));
    s -> type = S_BIN_OP;
    s -> o.dst = dst;
    s -> o.left = left;
    s -> o.right = right;
    s -> o.operator = type;
    return s;
}

cir_statement* cir_if_statement_new(char* condition, cir_statement_list* true_path, cir_statement_list* false_path) {
    cir_statement* s = xmalloc(sizeof(cir_statement));
    s -> type = S_IF;
    s -> i.condition = condition;
    s -> i.true_path = true_path;
    s -> i.false_path = false_path;
    return s;
}

void cir_statement_delete(cir_statement* s) {
    switch(s -> type) {
        case S_MOVE:
            xfree(s -> m.destination);
            xfree(s -> m.source);
            break;
        case S_LABEL:
            xfree(s -> l.name);
            cir_statement_list_delete(s -> l.statements);
            break;
        case S_JUMP:
            xfree(s -> j.label);
            break;
        case S_IF:
            xfree(s -> i.condition);
            cir_statement_list_delete(s -> i.true_path);
            cir_statement_list_delete(s -> i.false_path);
            break;
        case S_BIN_OP:
            xfree(s -> o.dst);
            xfree(s -> o.left);
            xfree(s -> o.right);
            break;
        default:
            break;
    }
    xfree(s);

}

cir_statement_list* cir_statement_list_new(uintmax_t capacity, uintmax_t growth) {
    cir_statement_list* l = list_new(capacity, growth);
    return l;
}

size_t cir_statement_list_size(cir_statement_list* l) {
    return list_size(l);
}

cir_statement* cir_statement_list_get(cir_statement_list* l, size_t index) {
    return (cir_statement*) list_get(l, index);
}

void cir_statement_list_delete(cir_statement_list* l) {
    if (l == NULL) { return; }
    for (size_t i = 0; i < cir_statement_list_size(l); i++) {
        cir_statement_delete(cir_statement_list_get(l, i));
    }
    list_delete(l);
}

void cir_statement_list_add(cir_statement_list* l, cir_statement* s) {
    list_append(l, s);
}

cir_statement_type cir_get_statement_type(cir_statement* s) {
    return s -> type;
}

char* cir_get_move_statement_destination(cir_statement* s) {
    return s -> m.destination;
}

cir_atom* cir_get_move_statement_source(cir_statement* s) {
    return s -> m.source;
}

char* cir_get_label_statement_name(cir_statement* s) {
    return s -> l.name;
}

size_t cir_label_statement_count(cir_statement* s) {
    return list_size(s -> l.statements);
}

cir_statement* cir_label_get_statement(cir_statement* s, size_t index) {
    return list_get(s -> l.statements, index);
}

char* operator_names[] = {
    "gt",
    "or",
    "add",
    "mod",
};

char* cir_binop_statement_operator_name(cir_statement* s) {
    return operator_names[s ->o.operator];
}

char* cir_binop_statement_destination(cir_statement* s) {
    return s -> o.dst;
}

char* cir_binop_statement_left(cir_statement* s) {
    return s -> o.left;
}
char* cir_binop_statement_right(cir_statement* s) {
    return s -> o.right;
}

char* cir_if_statement_condition(cir_statement* s) {
    return s -> i.condition;
}

size_t cir_if_get_true_statement_count(cir_statement* s) {
    return list_size(s -> i.true_path);
}

cir_statement* cir_if_get_true_statement(cir_statement* s, size_t index) {
    return list_get(s -> i.true_path, index);
}

size_t cir_if_get_false_statement_count(cir_statement* s) {
    return list_size(s -> i.false_path);
}

cir_statement* cir_if_get_false_statement(cir_statement* s, size_t index) {
    return list_get(s -> i.false_path, index);
}

char* cir_jump_get_label(cir_statement* s) {
    return s -> j.label;
}
