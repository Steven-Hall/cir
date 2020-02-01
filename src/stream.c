#include "stream.h"

typedef struct stream {
    FILE* source;
    char next_char;
    char current_char;
} stream;

stream* file_stream_new(FILE* source) {
    stream* s = malloc(sizeof(stream));
    s -> source = source;
    s -> next_char = fgetc(source);
    s -> current_char = fgetc(source);
    return s;
}

void stream_delete(stream* s) {
    free(s);
}

bool s_end(stream* s) {
    return s -> next_char == EOF;
}

char s_current_char(stream* s) {
    return s -> current_char;
}

char s_next_char(stream* s) {
    return s -> next_char;
}

void s_read_char(stream* s) {
    s -> current_char = s -> next_char;
    s -> next_char = fgetc(s -> source);
}
