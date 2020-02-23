#include "../include/cir.h"

#include "stream.h"
#include "lexer.h"
#include "parser.h"
#include "memory.h"

cir* cir_from_file(FILE* file) {
    stream* stream = file_stream_new(file);
    cir_lexer* lexer = cir_lexer_new(stream);
    cir_parser* parser = cir_parser_new(lexer);

    cir* cir = p_parse(parser);

    cir_parser_delete(parser);
    cir_lexer_delete(lexer);
    stream_delete(stream);

    return cir;
}

void cir_to_file(cir* cir, FILE* file) {
    char* string = cir_stringify(cir);
    fprintf(file, "%s\n", string);
    xfree(string);
}

cir* cir_from_string(char* source) {
    stream* stream = string_stream_new(source);
    cir_lexer* lexer = cir_lexer_new(stream);
    cir_parser* parser = cir_parser_new(lexer);

    cir* cir = p_parse(parser);

    cir_parser_delete(parser);
    cir_lexer_delete(lexer);
    stream_delete(stream);

    return cir;
}

char* cir_to_string(cir* cir) {
    char* string = cir_stringify(cir);
    return string;
}
