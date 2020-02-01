#include "lexer.h"

typedef struct lexer {
    cir_token current_token;
    stream* input;
} lexer;

lexer* lexer_new(stream* input) {
    lexer* l = malloc(sizeof(lexer));

    l -> current_token.type = CIR_INVALID;
    l -> current_token.value = NULL;
    l -> input = input;

    return l;
}

void lexer_delete(lexer* l) {
    free(l);
}

cir_token l_current_token(lexer* l) {
    return l -> current_token;
}

static char* invalid_token_value(stream* input) {
    // read 20 characters, or until the first whitespace character into the token value
    // then read from the stream until the next whitespace character

    // TODO: implement
    return NULL;
}

static void l_eat_whitespace(stream* input) {
    char next_char = s_next_char(input);
    while(next_char == '\n' || next_char == '\r' || next_char == ' ' || next_char == '\t') {
        s_read_char(input);
        next_char = s_next_char(input);
    }
}

void l_read_token(lexer* l) {
    stream* input = l -> input;
    l_eat_whitespace(input);
    char next_char = s_next_char(input);

    if (s_end(input)) {
        l -> current_token.type = CIR_END;
        l -> current_token.value = "";
        return;
    }

    switch(next_char) {
        default:
            l -> current_token.type = CIR_INVALID;
            l -> current_token.value = invalid_token_value(input);
    }
}
