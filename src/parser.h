#ifndef _CIR_PARSER_H_
#define _CIR_PARSER_H_

#include <stdarg.h>

#include "lexer.h"
#include "tokens.h"
#include "types.h"
#include "memory.h"
#include "string_utils.h"

typedef struct parser parser;

parser* parser_new(lexer* lexer);
void parser_delete(parser* p);

cir* p_parse(parser* p);

#endif
