#include <stdio.h>

#include "stream.h"
#include "lexer.h"
#include "parser.h"
#include "types.h"

int main(void) {
    stream* input_stream = file_stream_new(stdin);
    lexer* lexer = lexer_new(input_stream);
    parser* parser = parser_new(lexer);

    cir* ir = p_parse(parser);

    cir_delete(ir);
    parser_delete(parser);
    lexer_delete(lexer);
    stream_delete(input_stream);
    return 0;
}
