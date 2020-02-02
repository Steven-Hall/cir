#include "parser.h"

typedef struct parser {
    lexer* lexer;
} parser;

parser* parser_new(lexer* lexer) {
    parser* p = xmalloc(sizeof(parser));
    p -> lexer = lexer;
    return p;
}

void parser_delete(parser* p) {
    xfree(p);
}

// used after errors, seek the next sensible token
// to start reading from again
static void p_reset(parser* p) {
    lexer* lexer = p -> lexer;
    cir_token token = l_current_token(lexer);

    while(token.type != CIR_END && token.type != CIR_LPAREN) {
        l_read_token(lexer);
        token = l_current_token(lexer);
    }
}

#define MAX_ERROR_SIZE 100
static void handle_error(parser*p, cir* ir, char* error_message, ...) {
        char* buffer = xmalloc(sizeof(char) * (MAX_ERROR_SIZE+1));
        va_list args;
        va_start(args, error_message);
        vsnprintf(buffer, MAX_ERROR_SIZE, error_message, args);
        va_end(args);
        buffer[MAX_ERROR_SIZE] = 0;
        cir_add_error(ir, buffer);
        p_reset(p);
}

static bool p_expect(parser* p, cir* ir, cir_token_type expected_type) {
    cir_token_type current_type = l_current_token(p -> lexer).type;

    if(current_type != expected_type) {
        handle_error(p, ir, "expected token type %d but got %d", expected_type, current_type);
        return false;
    }
    return true;
}

static bool p_eat(parser* p, cir* ir, cir_token_type expected_type) {
    if (p_expect(p, ir, expected_type)) {
        l_read_token(p -> lexer);
        return true;
    }
    return false;
}

static cir_function_header* p_read_function_header(parser* p, cir* ir) {
    if(!p_eat(p, ir, CIR_LPAREN)) { return NULL; }
    if(!p_eat(p, ir, CIR_FUNCTION)) { return NULL; }

    if(!p_expect(p, ir, CIR_IDENTIFIER)) { return NULL; }
    char* token_value = l_current_token(p -> lexer).value;
    uint64_t name_length = strlen(token_value);
    char* function_name = xmalloc(sizeof(char) * name_length + 1);
    strncpy(function_name, token_value, name_length);

    function_name[name_length] = 0;
    l_read_token(p -> lexer);

    if(!p_eat(p, ir, CIR_RPAREN)) { return NULL; }

    cir_function_header* header = cir_function_header_new(function_name);
    return header;
}

static cir_function_body* p_read_function_body(parser* p, cir* ir) {
    cir_function_body* body = cir_function_body_new();

    if(!p_eat(p, ir, CIR_LPAREN)) { return NULL; };

    if(!p_eat(p, ir, CIR_RPAREN)) { return NULL; };

    return body;
}

static cir_function* p_read_function(parser* p, cir* ir) {
    if(!p_eat(p, ir, CIR_LPAREN)) { return NULL; }

    cir_function* function = cir_function_new();

    cir_function_header* header =  p_read_function_header(p, ir);

    if(header == NULL) {
        return function;
    }

    cir_function_body* body = p_read_function_body(p, ir);

    if(body == NULL) {
        return function;
    }

    if(!p_eat(p, ir, CIR_RPAREN)) { return NULL; }

    function -> header = header;
    function -> body = body;
    return function;
}

cir* p_parse(parser* p) {
    cir* ir = cir_new();
    lexer* lexer = p -> lexer;
    l_read_token(lexer);
    cir_token token = l_current_token(lexer);

    while(token.type != CIR_END) {
        if(token.type == CIR_INVALID) {
            char* error_message = xmalloc(sizeof(char) * 100);
            sprintf(error_message, "invalid_token %s, line: %ld, column: %ld", token.value, token.line, token.column);
            cir_add_error(ir, error_message);
            p_reset(p);
        }

        token = l_current_token(lexer);

        if (token.type != CIR_END) {
            cir_function* function = p_read_function(p, ir);
            if (function != NULL) {
                cir_add_function(ir, function);
            }
        }

        token = l_current_token(lexer);
        l_read_token(lexer);
    }

    return ir;
}
