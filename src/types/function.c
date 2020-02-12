#include "../cir.h"
#include "../memory.h"

typedef struct cir_function_header {
    char* function_name;
} cir_function_header;

typedef struct cir_function {
    cir_function_header* header;
    cir_statement_list* statements;
} cir_function;

cir_function_header* cir_function_header_new(char* name) {
    cir_function_header* h = xmalloc(sizeof(cir_function_header));
    h -> function_name = name;
    return h;
}

void cir_function_header_delete(cir_function_header* h) {
    if (h == NULL) { return; }
    xfree(h -> function_name);
    xfree(h);
}

cir_function* cir_function_new(cir_function_header* header, cir_statement_list* statements) {
    cir_function* f = xmalloc(sizeof(cir_function));
    f -> header = header;
    f -> statements = statements;
    return f;
}

void cir_function_delete(cir_function* f) {
    cir_function_header_delete(f -> header);
    cir_statement_list_delete(f -> statements);
    xfree(f);
}

char* cir_function_name(cir_function* f) {
    return f -> header -> function_name;
}
