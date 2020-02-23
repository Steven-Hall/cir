#include "stream.h"

typedef enum {
    FILE_STREAM,
    STRING_STREAM,
} cir_stream_type;

typedef struct stream {
    cir_stream_type type;
    char next_char;
    char current_char;
    size_t line;
    size_t column;
    size_t pos;

    union {
        FILE* f_source;
        char* s_source;
    };
} stream;

stream* file_stream_new(FILE* source) {
    stream* s = xmalloc(sizeof(stream));
    s -> type = FILE_STREAM;
    s -> f_source = source;
    s -> current_char = fgetc(source);
    s -> next_char = fgetc(source);
    s -> line = 1;
    s -> column = 1;
    return s;
}

stream* string_stream_new(char* source) {
   stream* s = xmalloc(sizeof(stream));
    s -> type = STRING_STREAM;
    s -> pos = 2;
    s -> s_source = source;

    size_t len = strlen(source);

    if (len > 0) {
        s -> current_char = source[0];
    } else {
        s -> current_char = EOF;
    }

    if (len > 1) {
        s -> next_char = source[1];
    } else {
        s -> next_char = EOF;
    }

    s -> line = 1;
    s -> column = 1;
    return s;

}

void stream_delete(stream* s) {
    xfree(s);
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
    if (s -> type == FILE_STREAM) {
        s -> current_char = s -> next_char;
        s -> next_char = fgetc(s -> f_source);
   } else {
        s -> current_char = s -> next_char;
        s -> next_char = s -> s_source[s -> pos];
        s -> pos++;
    }

    if (s -> current_char == '\n') {
        s -> line++;
        s -> column = 0;
    } else {
        s -> column++;
    }
}
