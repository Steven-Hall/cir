#include "lexer.h"

typedef struct lexer {
    cir_token current_token;
    stream* input;
} lexer;

lexer* lexer_new(stream* input) {
    lexer* l = malloc(sizeof(lexer));

    l -> current_token.type = CIR_START;
    l -> current_token.value = NULL;
    l -> current_token.line = 1;
    l -> current_token.column = 1;
    l -> input = input;

    return l;
}

void lexer_delete(lexer* l) {
    free(l -> current_token.value);
    free(l);
}

cir_token l_current_token(lexer* l) {
    return l -> current_token;
}

static bool is_whitespace(char c) {
    return (c == '\n' || c == '\r' || c == ' ' || c == '\t');
}

static char* invalid_token_value(stream* input) {
    // read 20 characters, or until the first whitespace character into the token value
    // then read from the stream until the next whitespace character
    char* value = malloc(sizeof(char) * 24);
    value[0] = s_current_char(input);
    s_read_char(input);
    char next_char = s_current_char(input);

    int i = 1;
    while(!is_whitespace(next_char) && !s_end(input) && i < 20) {
        value[i] = next_char;
        i++;
        s_read_char(input);
        next_char = s_current_char(input);
    }

    // if the full 20 chars are used then add '...' at the end
    if (i == 20) {
        value[20] = '.';
        value[21] = '.';
        value[22] = '.';
        value[23] = 0;
    } else {
        value[i] = 0;
    }

    while(!is_whitespace(next_char) && !s_end(input)) {
        s_read_char(input);
        next_char = s_current_char(input);
    }

    return value;
}

static void l_eat_whitespace(stream* input) {
    char c = s_current_char(input);

    while(is_whitespace(c) && !s_end(input)) {
        s_read_char(input);
        c = s_current_char(input);
    }
}

void l_read_token(lexer* l) {
    free(l -> current_token.value);
    stream* input = l -> input;
    l_eat_whitespace(input);
    char next_char = s_current_char(input);

    if (s_end(input)) {
        l -> current_token.type = CIR_END;
        l -> current_token.value = NULL;
        l -> current_token.line = s_line(input);
        l -> current_token.column = s_column(input);
        return;
    }

    switch(next_char) {
        default:
            l -> current_token.type = CIR_INVALID;
            l -> current_token.line = s_line(input);
            l -> current_token.column = s_column(input);
            l -> current_token.value = invalid_token_value(input);
   }
}
