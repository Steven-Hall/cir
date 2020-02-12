#ifndef CIR_STRINGIFY_H_
#define CIR_STRINGIFY_H_

#include "cir.h"

typedef char cir_string;

cir_string* cir_stringify(cir* cir);
void cir_string_delete(cir_string* string);

#endif
