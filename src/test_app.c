#include <stdio.h>

#include "stream.h"
#include "lexer.h"
#include "parser.h"
#include "cir.h"

int main(void) {
    int exit_code = 0;

    stream* input_stream = file_stream_new(stdin);
    lexer* lexer = lexer_new(input_stream);
    parser* parser = parser_new(lexer);
    cir* ir = p_parse(parser);

    if(cir_error_count(ir) > 0) {
        fprintf(stderr, "\n--- ERRORS ---\n");
        for (size_t i = 0; i < cir_error_count(ir); i++) {
            fprintf(stderr, "%s\n", cir_get_error(ir, i));
        }
        fprintf(stderr, "\n");
        exit_code = 1;
    } else {
        printf("parsed successfully\n");
    }

    cir_delete(ir); 
    parser_delete(parser);
    lexer_delete(lexer);
    stream_delete(input_stream);
    return exit_code;
}
