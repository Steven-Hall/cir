#include <stdio.h>

#include "../include/cir.h"

int main(void) {
    int exit_code = 0;

    cir* ir = cir_from_file(stdin);

    if(cir_error_count(ir) > 0) {
        fprintf(stderr, "\n--- ERRORS ---\n");
        for (size_t i = 0; i < cir_error_count(ir); i++) {
            fprintf(stderr, "%s\n", cir_get_error(ir, i));
        }
        fprintf(stderr, "\n");
        exit_code = 1;
    }

    cir_to_file(ir, stdout);
    cir_delete(ir); 
    return exit_code;
}
