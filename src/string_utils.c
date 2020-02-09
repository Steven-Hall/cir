#include "string_utils.h"

#include <stdio.h>

char* s_copy(char* source) {
    size_t buffer_size = strlen(source);
    char* buffer = xmalloc(sizeof(char) * buffer_size + 1);
    strncpy(buffer, source, buffer_size);
    buffer[buffer_size] = 0;
    return buffer;
}

bool s_xmatch(char* actual, char* expected) {
    size_t expected_size = strlen(expected);
    size_t actual_size = strlen(actual);

    if (actual_size != expected_size) {
        return false;
    }
    return (strncmp(actual, expected, expected_size) == 0);
}
