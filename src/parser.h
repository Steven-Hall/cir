#ifndef _CIR_PARSER_H_
#define _CIR_PARSER_H_

#include "lexer.h"
#include "../include/cir.h"

typedef struct cir_parser cir_parser;

cir_parser* cir_parser_new(cir_lexer* lexer);
void cir_parser_delete(cir_parser* p);

cir* p_parse(cir_parser* p);

#endif
