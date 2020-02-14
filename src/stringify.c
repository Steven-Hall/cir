#include "cir.h"

#include <stdio.h>
#include "string_utils.h"

// TODO measure and optmize performance, lots of string copying/mallocing happening here
// TODO manage buffer size being exceeded

static char* cir_statement_stringify(cir_statement* statement) {
    uintmax_t length = 0;
    char* buffer = malloc(sizeof(char) * 1000);
    buffer[0] = '(';
    length++;

    buffer[length] = ')';
    length++;
    buffer[length] = 0;
    return buffer;
}

static char* cir_function_stringify(cir_function* function) {
    uintmax_t length = 0;
    char* buffer = malloc(sizeof(char) * 1000);
    buffer[0] = '(';
    length++;
 
    strncpy(buffer + length, "function ", strlen("function ") + 1);
    length += 9;

    uintmax_t name_length = strlen(cir_function_name(function));
    strncpy((buffer + length), cir_function_name(function), name_length);
    length += name_length;

    // TODO eventually have to handle args properly
    strncpy((buffer + length), " ()", 4);
    length += 3;

    for (size_t i = 0; i < cir_statement_count(function); i++) {
        char* statement_string = cir_statement_stringify(cir_get_statement(function, i));
        printf("%s\n", statement_string);
    }

    buffer[length] = ')';
    length++;
    buffer[length] = 0;
    return buffer;
}

char* cir_stringify(cir* cir) {
    char* buffer = malloc(sizeof(char) * 1000);
    uintmax_t length = 0;
    for(size_t i = 0; i < cir_function_count(cir); i++) {
        char* function_string = cir_function_stringify(cir_get_function(cir, i));
        length += strlen(function_string);
        strncpy(buffer, function_string, length);
    }
    buffer[length] = 0;
    return buffer;
}
