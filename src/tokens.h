#ifndef _IR_TOKENS_H_
#define _IR_TOKENS_H_

typedef enum {
    CIR_INVALID,
    CIR_END,
    CIR_INTEGER
} cir_token_type;

typedef struct cir_token {
    cir_token_type type;
    char* value;
} cir_token;

cir_token* token_new(cir_token_type type, char* value);
void token_delete(cir_token* t);

#endif
