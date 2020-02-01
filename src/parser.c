#include "parser.h"

typedef struct parser {
    lexer* lexer;
} parser;

parser* parser_new(lexer* lexer) {
    parser* p = malloc(sizeof(parser));
    p -> lexer = lexer;
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

        if(token.type == CIR_INVALID) {
            char* error_message = malloc(sizeof(char) * 100);
            sprintf(error_message, "invalid_token %s, line: %ld, column: %ld", token.value, token.line, token.column);
            cir_add_error(ir, error_message);
        }
    }

    return ir;
}
