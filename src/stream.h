#ifndef _CIR_STREAM_H_
#define _CIR_STREAM_H_

#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>

#include "memory.h"

typedef struct stream stream;

stream* file_stream_new(FILE* source);
void stream_delete(stream* s);

void s_read_char(stream* s);
bool s_end(const stream* s);
char s_current_char(const stream* s);
char s_next_char(const stream* s);
uintmax_t s_line(const stream* s);
uintmax_t s_column(const stream* s);

#endif
