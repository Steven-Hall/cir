#include "lexer.h"

typedef struct lexer {
    cir_token current_token;
    cir_token next_token;
    stream* input;
} lexer;

lexer* lexer_new(stream* input) {
    lexer* l = xmalloc(sizeof(lexer));

    // this is about to get freed by l_read_token()
    // so make it explicitly null to avoid problems
    // and keep valgrind happy
    l -> current_token.value = NULL;

    l -> next_token.type = CIR_START;
    l -> next_token.value = NULL;
    l -> next_token.line = 1;
    l -> next_token.column = 1;
    l -> input = input;

    l_read_token(l);

    return l;
}

void lexer_delete(lexer* l) {
    xfree(l -> current_token.value);
    xfree(l -> next_token.value);
    xfree(l);
}

cir_token l_current_token(lexer* l) {
    return l -> current_token;
}

cir_token l_next_token(lexer* l) {
    return l -> next_token;
}

static bool is_whitespace(char c) {
    return (c == '\n' || c == '\r' || c == ' ' || c == '\t');
}

static char* invalid_token_value(stream* input) {
    // read 20 characters, or until the first whitespace character into the token value
    // then read from the stream until the next whitespace character
    char* value = xmalloc(sizeof(char) * 24);
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
    xfree(l -> current_token.value);
    l -> current_token.type = l -> next_token.type;
    l -> current_token.value = l -> next_token.value;
    l -> current_token.line = l -> next_token.line;
    l -> current_token.column = l -> next_token.column;

    stream* input = l -> input;
    l_eat_whitespace(input);
    char next_char = s_current_char(input);

    if (s_end(input)) {
        l -> next_token.type = CIR_END;
        l -> next_token.value = NULL;
        l -> next_token.line = s_line(input);
        l -> next_token.column = s_column(input);
        return;
    }

    switch(next_char) {
        default:
            l -> next_token.type = CIR_INVALID;
            l -> next_token.line = s_line(input);
            l -> next_token.column = s_column(input);
            l -> next_token.value = invalid_token_value(input);
   }
}
