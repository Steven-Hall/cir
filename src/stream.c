#include "stream.h"

typedef struct stream {
    FILE* source;
    char next_char;
    char current_char;
    uintmax_t line;
    uintmax_t column;
} stream;

stream* file_stream_new(FILE* source) {
    stream* s = malloc(sizeof(stream));
    s -> source = source;
    s -> current_char = fgetc(source);
    s -> next_char = fgetc(source);
    s -> line = 1;
    s -> column = 1;
    return s;
}

void stream_delete(stream* s) {
    free(s);
}

bool s_end(const stream* s) {
    return s -> current_char == EOF;
}

char s_current_char(const stream* s) {
    return s -> current_char;
}

uintmax_t s_line(const stream* s) {
    return s -> line;
}

uintmax_t s_column(const stream* s) {
    return s -> column;
}

char s_next_char(const stream* s) {
    return s -> next_char;
}

void s_read_char(stream* s) {
    s -> current_char = s -> next_char;
    s -> next_char = fgetc(s -> source);
    if (s -> current_char == '\n') {
        s -> line++;
        s -> column = 0;
    } else {
        s -> column++;
    }
}
