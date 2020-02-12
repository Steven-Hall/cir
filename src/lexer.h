#ifndef _CIR_LEXER_H_
#define _CIR_LEXER_H_

#include <string.h>

#include "string_utils.h"
#include "stream.h"
#include "tokens.h"
#include "memory.h"

typedef struct cir_lexer cir_lexer;

cir_lexer* cir_lexer_new(stream* input);
void cir_lexer_delete(cir_lexer* l);

uintmax_t l_paren_balance(cir_lexer* l);
cir_token l_current_token(cir_lexer* l);
cir_token l_next_token(cir_lexer* l);
void l_read_token(cir_lexer* l);

#endif
