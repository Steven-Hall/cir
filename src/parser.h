#ifndef _CIR_PARSER_H_
#define _CIR_PARSER_H_

#include "lexer.h"
#include "cir.h"

typedef struct parser parser;

parser* parser_new(lexer* lexer);
void parser_delete(parser* p);

cir* p_parse(parser* p);

#endif
