#ifndef _IR_TOKENS_H_
#define _IR_TOKENS_H_

#include <stdint.h>

typedef enum {
    CIR_INVALID,
    CIR_START,
    CIR_END,
    CIR_LPAREN,
    CIR_INTEGER
} cir_token_type;

typedef struct cir_token {
    cir_token_type type;
    char* value;
    uint64_t line;
    uint64_t column;
} cir_token;

#endif
