#include "parser.h"

typedef struct parser {
    lexer* lexer;
} parser;

parser* parser_new(lexer* lexer) {
    parser* p = malloc(sizeof(parser));
    p -> lexer = lexer;
    l_read_token(lexer);
    return p;
}

void parser_delete(parser* p) {
    free(p);
}

cir* p_parse(parser* p) {
    cir* ir = cir_new();

    lexer* lexer = p -> lexer;

    cir_token token = l_current_token(lexer);
    while(token.type != CIR_END) {

        l_read_token(lexer);
        token = l_current_token(lexer);
    }

    return ir;
}
