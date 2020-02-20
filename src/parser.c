#include "parser.h"

#include <stdarg.h>

typedef struct cir_parser {
    cir_lexer* lexer;
} cir_parser;

cir_parser* cir_parser_new(cir_lexer* lexer) {
    cir_parser* p = xmalloc(sizeof(cir_parser));
    p -> lexer = lexer;
    return p;
}

void cir_parser_delete(cir_parser* p) {
    xfree(p);
}

static void p_reset(cir_parser* p) {
    cir_lexer* lexer = p -> lexer;
    cir_token token = l_next_token(lexer);

    // paren balance counts the ratio of left parens to right
    // the ration == 0 is a reasonable guess at where to continue
    // reading
    while(token.type != CIR_END && l_paren_balance(lexer) != 0) {
        l_read_token(lexer);
        token = l_next_token(lexer);
    }
}

#define MAX_ERROR_SIZE 100
static void handle_error(cir_parser*p, cir* ir, char* error_message, ...) {
        char* buffer = xmalloc(sizeof(char) * (MAX_ERROR_SIZE+1));
        va_list args;
        va_start(args, error_message);
        vsnprintf(buffer, MAX_ERROR_SIZE, error_message, args);
        va_end(args);
        buffer[MAX_ERROR_SIZE] = 0;
        cir_add_error(ir, buffer);
        p_reset(p);
}

static bool p_expect(cir_parser* p, cir* ir, cir_token_type expected_type) {
    cir_token current_token = l_current_token(p -> lexer);

    if(current_token.type != expected_type) {
        handle_error(p, ir, "unexpected_token: expected: %s actual: %s value: %s line: %d column: %d",
                cir_token_names[expected_type], cir_token_names[current_token.type],
                current_token.value, current_token.line, current_token.column);
        return false;
    }
    return true;
}

static bool p_eat(cir_parser* p, cir* ir, cir_token_type expected_type) {
    if (p_expect(p, ir, expected_type)) {
        l_read_token(p -> lexer);
        return true;
    }
    return false;
}

static cir_function_header* p_read_function_header(cir_parser* p, cir* ir) {
    if(!p_eat(p, ir, CIR_FUNCTION)) { return NULL; }

    char* function_name = s_copy(l_current_token(p -> lexer).value);
    l_read_token(p -> lexer);

    // TODO read function args

    if(!p_eat(p, ir, CIR_LPAREN)) { 
        xfree(function_name);
        return NULL;
    }
    
    if(!p_eat(p, ir, CIR_RPAREN)) {
        xfree(function_name);
        return NULL;
    }

    cir_function_header* header = cir_function_header_new(function_name);

    return header;
}

static cir_atom* p_read_atom(cir_parser* p, cir* ir) {
    cir_token token = l_current_token(p -> lexer);
    char* identifier = s_copy(token.value);
 
    switch(token.type) {
        case CIR_IDENTIFIER:
            l_read_token(p -> lexer);
           return cir_identifier_atom_new(identifier);
        case CIR_INTEGER:
            l_read_token(p -> lexer);
            uint64_t value = atoi(identifier);
            xfree(identifier);
            return cir_integer_atom_new(value);
        default:
            handle_error(p, ir, "unexpected_token: expected: CIR_IDENTIFIER | CIR_INTEGER actual: %s", cir_token_names[token.type]);
            return NULL;
    }
}

static cir_statement* p_read_move(cir_parser* p, cir* ir) {
    //TODO better error handling

    l_read_token(p -> lexer);
    char* dst = s_copy(l_current_token(p -> lexer).value);
    l_read_token(p -> lexer);
    cir_atom* src = p_read_atom(p, ir);

    if (!p_eat(p, ir, CIR_RPAREN)) {
        xfree(dst);
        cir_atom_delete(src);
        return NULL;
    };

    return cir_move_statement_new(dst, src);
}

// forward declaration, definition is below
static cir_statement_list* p_read_statement_list(cir_parser* p, cir* ir);

static cir_statement* p_read_label(cir_parser* p, cir* ir) {
    //TODO better error handling
    
    l_read_token(p -> lexer);
    char* label = s_copy(l_current_token(p -> lexer).value);

    l_read_token(p -> lexer);

    cir_statement_list* statements = p_read_statement_list(p, ir);

    if(!p_eat(p, ir, CIR_RPAREN)) {
        xfree(label);
        return NULL;
    };

    return cir_label_statement_new(label, statements);
}

static cir_statement* p_read_jump(cir_parser* p, cir* ir) {
    // TODO better error handling

    l_read_token(p -> lexer);
    char* label = s_copy(l_current_token(p -> lexer).value);

    l_read_token(p -> lexer);
    if (!p_eat(p, ir, CIR_RPAREN)) { return NULL; }

    return cir_jump_statement_new(label);
}

static cir_statement* p_read_bin_operator(cir_parser* p, cir* ir, cir_token_type type) {
    // TODO better error handling

    l_read_token(p -> lexer);
    char* dst = s_copy(l_current_token(p -> lexer).value);
    l_read_token(p -> lexer);
    char* left = s_copy(l_current_token(p -> lexer).value);
    l_read_token(p -> lexer);
    char* right = s_copy(l_current_token(p -> lexer).value);

    cir_operator op_type;

    switch(type) {
        case CIR_GT:
            op_type = S_GT;
            break;
        case CIR_MOD:
            op_type = S_MOD;
            break;
        case CIR_OR:
            op_type = S_OR;
            break;
        case CIR_ADD:
            op_type = S_ADD;
            break;
        default:
            //TODO add error case
            break;
    }

    l_read_token(p -> lexer);
    if (!p_eat(p, ir, CIR_RPAREN)) { return NULL; }

    return cir_binop_statement_new(dst, left, right, op_type);
}

static cir_statement* p_read_if(cir_parser* p, cir* ir) {
    // TODO error handling
    l_read_token(p -> lexer);

    // read condition identifier
    char* condition = s_copy(l_current_token(p -> lexer).value);
    l_read_token(p -> lexer);

    cir_statement_list* true_path = p_read_statement_list(p, ir);

    cir_statement_list* false_path = p_read_statement_list(p, ir);

    if(!p_eat(p, ir, CIR_RPAREN)) { return NULL; }

    return cir_if_statement_new(condition, true_path, false_path);
}

static cir_statement* p_read_statement(cir_parser* p, cir* ir) {
    cir_token t = l_current_token(p -> lexer);
    if (t.type == CIR_RPAREN) {
        return NULL;
    }
    
    if (!p_eat(p, ir, CIR_LPAREN)) { return NULL; }

    t = l_current_token(p -> lexer);
    switch(t.type) {
        case CIR_RETURN:
            l_read_token(p -> lexer);
            p_eat(p, ir, CIR_RPAREN);
            return cir_return_statement_new();
        case CIR_MOVE:
            return p_read_move(p, ir);
        case CIR_LABEL:
            return p_read_label(p, ir);
        case CIR_JUMP:
            return p_read_jump(p, ir);
        case CIR_GT:
        case CIR_MOD:
        case CIR_OR:
        case CIR_ADD:
            return p_read_bin_operator(p, ir, t.type);
        case CIR_IF:
            return p_read_if(p, ir);
        default:
            handle_error(p, ir, "unexpected_operation: %s line: %d, column: %d", cir_token_names[t.type], t.line, t.column);
            return NULL;
    }
}

static cir_statement_list* p_read_statement_list(cir_parser* p, cir* ir) {
    if(!p_eat(p, ir, CIR_LPAREN)) { return NULL; };

    cir_statement_list* l = cir_statement_list_new(10, 10);

    cir_statement* s = p_read_statement(p, ir);
    while (s != NULL) {
        cir_statement_list_add(l, s);
        s = p_read_statement(p, ir);
    }

    if(!p_eat(p, ir, CIR_RPAREN)) { 
        cir_statement_list_delete(l);
        return NULL;
    };

    return l;
}

static cir_function* p_read_function(cir_parser* p, cir* ir) {
    if(!p_eat(p, ir, CIR_LPAREN)) { return NULL; }

    cir_function_header* header =  p_read_function_header(p, ir);
    if(header == NULL) {
        cir_function_header_delete(header);
        return NULL;
    }

    cir_statement_list* statements = p_read_statement_list(p, ir);
    if(statements == NULL) {
        cir_function_header_delete(header);
        cir_statement_list_delete(statements);
        return NULL;
    }

    if(!p_eat(p, ir, CIR_RPAREN)) { 
        cir_function_header_delete(header);
        cir_statement_list_delete(statements);
        return NULL;
    }

    return cir_function_new(header, statements);
}

cir* p_parse(cir_parser* p) {
    cir* ir = cir_new();
    cir_lexer* lexer = p -> lexer;
    l_read_token(lexer);
    cir_token token = l_current_token(lexer);

    while(token.type != CIR_END) {
        if(token.type == CIR_INVALID) {
            char* error_message = xmalloc(sizeof(char) * 100);
            sprintf(error_message, "invalid_token: %s, value: %s, line: %ld, column: %ld", cir_token_names[token.type], token.value, token.line, token.column);
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
