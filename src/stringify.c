#include "../include/cir.h"

#include <stdio.h>
#include "string_utils.h"

#define INDENT_SPACES 2

// TODO measure and optmize performance, lots of string copying/xmallocing happening here
// TODO manage buffer size being exceeded

static char* cir_move_statement_stringify(cir_statement* statement, size_t indent) {
    uintmax_t length = 0;
    char* buffer = xmalloc(sizeof(char) * 1000);

    for(size_t i = 0; i < indent * INDENT_SPACES; i++) {
        buffer[length] = ' ';
        length++;
    }
 
    strncpy((buffer + length), "(move ", 7);
    length += 6;

    char* destination = cir_get_move_statement_destination(statement);
    size_t destination_length = strlen(destination);
    strncpy((buffer + length), destination, destination_length + 1);
    length += destination_length;
    buffer[length] = ' ';
    length ++;

    cir_atom* source_atom = cir_get_move_statement_source(statement);
    char* source = cir_atom_string_value(source_atom);

    size_t source_length = strlen(source);
    strncpy((buffer + length), source, source_length +1);
    length += source_length;

    buffer[length] = ')';
    length++;
    buffer[length] = 0;
    return buffer;
}


// TOOD reorganize
static char* cir_statement_stringify(cir_statement* statement, size_t indent);

static char* cir_label_statement_stringify(cir_statement* statement, size_t indent) {
    uintmax_t length = 0;
    char* buffer = xmalloc(sizeof(char) * 1000);

    for (size_t i = 0; i < indent * INDENT_SPACES; i++) {
        buffer[length] = ' ';
        length++;
    }
 
    strncpy((buffer + length), "(label ", 8);
    length += 7;

    char* label_name = cir_get_label_statement_name(statement);
    size_t name_length = strlen(label_name);

    strncpy((buffer + length), label_name, name_length);
    length += name_length;
    buffer[length] = ' ';
    length++;
    buffer[length] = '(';
    length++;

    if (cir_label_statement_count(statement) > 0) {
        buffer[length] = '\n';
        length++;
    }

    for(size_t i = 0; i < cir_label_statement_count(statement); i++) {
        char* inner_statement = cir_statement_stringify(cir_label_get_statement(statement, i), indent + 1);
        strncpy((buffer + length), inner_statement, strlen(inner_statement));
        length += strlen(inner_statement);
        buffer[length] = '\n';
        length++;
    }


    if (cir_label_statement_count(statement) > 0 ) {
        for (size_t i = 0; i < indent * INDENT_SPACES; i++) {
            buffer[length] = ' ';
            length++;
        }
    }

    buffer[length] = ')';
    buffer[length+1] = ')';
    length+=2;

    buffer[length] = 0;
    return buffer;
}

static char* cir_binop_statement_stringify(cir_statement* statement, size_t indent) {
    uintmax_t length = 0;
    char* buffer = xmalloc(sizeof(char) * 1000);

    for(size_t i = 0; i < indent * INDENT_SPACES; i++) {
        buffer[length] = ' ';
        length++;
    }

    buffer[length] = '(';
    length++;
 
    char* operator_name = cir_binop_statement_operator_name(statement);
    strncpy((buffer + length), operator_name, strlen(operator_name));
    length += strlen(operator_name);
    buffer[length] = ' ';
    length++;

    char* destination = cir_binop_statement_destination(statement);
    strncpy((buffer + length), destination, strlen(destination));
    length += strlen(destination);
    buffer[length] = ' ';
    length++;

    char* left = cir_binop_statement_left(statement);
    strncpy((buffer + length), left, strlen(left));
    length += strlen(left);
    buffer[length] = ' ';
    length++;

    char* right = cir_binop_statement_right(statement);
    strncpy((buffer + length), right, strlen(right));
    length += strlen(right);
    buffer[length] = ')';
    length++;

    buffer[length] = 0;

    return buffer;
}

static char* cir_if_statement_stringify(cir_statement* statement, size_t indent) {
    uintmax_t length = 0;
    char* buffer = xmalloc(sizeof(char) * 1000);

    for (size_t i = 0; i < indent * INDENT_SPACES; i++) {
        buffer[length] = ' ';
        length++;
    }

    strncpy((buffer + length), "(if ", 5);
    length += 4;

    char* condition = cir_if_statement_condition(statement);
    strncpy((buffer + length), condition, strlen(condition));
    length += strlen(condition);
    buffer[length] = '\n';
    length++;
 
    for (size_t i = 0; i < (indent + 1) * INDENT_SPACES; i++) {
        buffer[length] = ' ';
        length++;
    }

    buffer[length] = '(';
    length++;

    if (cir_if_get_true_statement_count(statement) > 1) {
        buffer[length] = '\n';
        length++;

        for(size_t i = 0; i < cir_if_get_true_statement_count(statement); i++) {
            char* s = cir_statement_stringify(cir_if_get_true_statement(statement, i), indent);
            strncpy((buffer + length), s, strlen(s));
            length += strlen(s);
            buffer[length] = '\n';
            length++;
        }

        for (size_t i = 0; i < indent * INDENT_SPACES; i++) {
            buffer[length] = ' ';
            length++;
        }
    } else {
        char* s = cir_statement_stringify(cir_if_get_true_statement(statement, 0), 0);
        strncpy((buffer + length), s, strlen(s));
        length += strlen(s);
    }

    buffer[length] = ')';

    buffer[length+1] = '\n';
    length+=2;

    for (size_t i = 0; i < (indent + 1) * INDENT_SPACES; i++) {
        buffer[length] = ' ';
        length++;
    }

    buffer[length] = '(';
    length++;

    if (cir_if_get_false_statement_count(statement) > 1) {
        buffer[length] = '\n';
        length++;

        for(size_t i = 0; i < cir_if_get_false_statement_count(statement); i++) {
            char* s = cir_statement_stringify(cir_if_get_false_statement(statement, i), indent + 2);
            strncpy((buffer + length), s, strlen(s));
            length += strlen(s);
            buffer[length] = '\n';
            length++;
        }

        for (size_t i = 0; i < (indent + 1) * INDENT_SPACES; i++) {
            buffer[length] = ' ';
            length++;
        }
    } else {
        char* s = cir_statement_stringify(cir_if_get_false_statement(statement, 0), 0);
        strncpy((buffer + length), s, strlen(s));
        length += strlen(s);
    }


    buffer[length] = ')';
    buffer[length + 1] = '\n';
    length += 2;

    for (size_t i = 0; i < indent * INDENT_SPACES; i++) {
        buffer[length] = ' ';
        length++;
    }
 
    buffer[length] = ')';
    length++;

    buffer[length] = 0;
    return buffer;
}

static char* cir_jump_statement_stringify(cir_statement* statement, size_t indent) {
    uintmax_t length = 0;
    char* buffer = xmalloc(sizeof(char) * 1000);

    for (size_t i = 0; i < indent * INDENT_SPACES; i++) {
        buffer[length] = ' ';
        length++;
    }

    strncpy((buffer + length), "(jump ", 7);
    length += 6;

    char* label = cir_jump_get_label(statement);
    strncpy(buffer + length, label, strlen(label));
    length += strlen(label);

    buffer[length] = ')';
    length++;
    buffer[length] = 0;

    return buffer;
}

static char* cir_statement_stringify(cir_statement* statement, size_t indent) {
    uintmax_t length = 0;
    char* buffer = xmalloc(sizeof(char) * 1000);
    
    char* s = "";

    switch(cir_get_statement_type(statement)) {
        case S_MOVE:
            s = cir_move_statement_stringify(statement, indent);
            break;
        case S_LABEL:
            s = cir_label_statement_stringify(statement, indent);
            break;
        case S_BIN_OP:
            s = cir_binop_statement_stringify(statement, indent);
            break;
        case S_IF:
            s = cir_if_statement_stringify(statement, indent);
            break;
        case S_JUMP:
            s = cir_jump_statement_stringify(statement, indent);
            break;
        default:
            printf("%d unhandled\n", cir_get_statement_type(statement));
            break;
    }

    size_t s_length = strlen(s);
    strncpy((buffer + length), s, s_length);
    length += s_length;
    buffer[length] = 0;
    return buffer;
}

static char* cir_function_stringify(cir_function* function) {
    uintmax_t length = 0;
    char* buffer = xmalloc(sizeof(char) * 1000);
    buffer[0] = '(';
    length++;
 
    strncpy(buffer + length, "function ", strlen("function ") + 1);
    length += 9;

    uintmax_t name_length = strlen(cir_function_name(function));
    strncpy((buffer + length), cir_function_name(function), name_length);
    length += name_length;

    // TODO eventually have to handle args properly
    strncpy((buffer + length), " () (", 6);
    length += 5;
    buffer[length] = '\n';
    length++;

    for (size_t i = 0; i < cir_statement_count(function); i++) {
        char* statement_string = cir_statement_stringify(cir_get_statement(function, i), 1);
        strncpy((buffer + length), statement_string, strlen(statement_string));
        length += strlen(statement_string);
        buffer[length] = '\n';
        length++;
    }

    buffer[length] = ')';
    length++;
    buffer[length] = ')';
    length++;
    buffer[length] = 0;
    return buffer;
}

char* cir_stringify(cir* cir) {
    char* buffer = xmalloc(sizeof(char) * 1000);
    uintmax_t length = 0;
    for(size_t i = 0; i < cir_function_count(cir); i++) {
        char* function_string = cir_function_stringify(cir_get_function(cir, i));
        length += strlen(function_string);
        strncpy(buffer, function_string, length);
    }
    buffer[length] = 0;
    return buffer;
}
