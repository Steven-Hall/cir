#ifndef _IR_TOKENS_H_
#define _IR_TOKENS_H_

#include <stdint.h>

#define FOREACH_TOKEN(TOKEN)   \
        TOKEN(CIR_INVALID)     \
        TOKEN(CIR_START)       \
        TOKEN(CIR_END)         \
        TOKEN(CIR_LPAREN)      \
        TOKEN(CIR_RPAREN)      \
        TOKEN(CIR_INTEGER)     \
        TOKEN(CIR_IDENTIFIER)  \
        TOKEN(CIR_FUNCTION)    \
        TOKEN(CIR_RETURN)      \
        TOKEN(CIR_MOVE)        \
        TOKEN(CIR_LABEL)       \
        TOKEN(CIR_JUMP)        \
        TOKEN(CIR_ADD)         \
        TOKEN(CIR_GT)          \
        TOKEN(CIR_IF)          \
        TOKEN(CIR_MOD)         \
        TOKEN(CIR_OR)          \

#define GENERATE_ENUM(ENUM) ENUM,
#define GENERATE_STRING(STRING) #STRING,

typedef enum {
    FOREACH_TOKEN(GENERATE_ENUM)
} cir_token_type;

extern const char* cir_token_names[];

typedef struct cir_token {
    cir_token_type type;
    char* value;
    uintmax_t line;
    uintmax_t column;
} cir_token;

#endif
