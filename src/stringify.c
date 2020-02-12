#include "stringify.h"

#include <stddef.h>
#include <string.h>

#include "cir.h"
#include "memory.h"

cir_string* cir_string_new(const char* value) {
    size_t length = strlen(value);
    cir_string* string = xmalloc((sizeof(char) * (length + 1)) + sizeof(size_t));
    *string = length;

    string = string + (sizeof(size_t));
    strncpy(string, value, length);
    string[length] = 0;

    return string;
}

static inline size_t cir_string_length(cir_string* string) {
    return (size_t)(*(string - sizeof(size_t)));
}

void cir_string_delete(cir_string* string) {
    string = (string - sizeof(size_t));
    xfree(string);
}

static void cir_string_cat(cir_string* dst, cir_string* src) {
    size_t dst_length = cir_string_length(dst);
    size_t src_length = cir_string_length(src);
    size_t new_length = dst_length + src_length;

    cir_string* dst_string = dst - sizeof(size_t);
    dst_string = xrealloc(dst_string, new_length + 1 + sizeof(size_t));
    *dst_string = new_length;
    dst_string = dst_string + sizeof(size_t);
    strncpy(dst_string + (dst_length * sizeof(char)), src, src_length);
    dst_string[new_length] = 0;
}

static cir_string* cir_function_stringify(cir_function* function) {
    cir_string* function_string = cir_string_new("(");
    cir_string* function_name = cir_string_new(cir_function_name(function));
    cir_string* args = cir_string_new(" ()");
    cir_string* function_end = cir_string_new(")");

    cir_string_cat(function_string, function_name);
    cir_string_cat(function_string, args);
    cir_string_cat(function_string, function_end);

    return function_string;
}

cir_string* cir_stringify(cir* cir) {
    cir_string* buffer = cir_string_new("");

    for(size_t i = 0; i < cir_function_count(cir); i++) {
        cir_string* function_string = cir_function_stringify(cir_get_function(cir, i));
        cir_string_cat(buffer, function_string);
        cir_string_delete(function_string);
    }
    return buffer;
}
