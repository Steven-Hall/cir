#ifndef _CIR_LEXER_H_
#define _CIR_LEXER_H_

#include <stdlib.h>

#include "stream.h"
#include "tokens.h"

typedef struct lexer lexer;

lexer* lexer_new(stream* input);
void lexer_delete(lexer* l);

cir_token l_current_token(lexer* l);
void l_read_token(lexer* l);

#endif