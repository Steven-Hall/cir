#ifndef _STRING_UTILS_H_
#define _STRING_UTILS_H_

#include <stddef.h>
#include <string.h>
#include <stdbool.h>

#include "memory.h"

char* s_copy(char* source);
bool s_xmatch(char* actual, char* expected);

#endif
