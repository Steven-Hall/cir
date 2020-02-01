#ifndef _CIR_STREAM_H_
#define _CIR_STREAM_H_

#include "stdbool.h"
#include "stdlib.h"
#include "stdio.h"

typedef struct stream stream;

stream* file_stream_new(FILE* source);
void stream_delete(stream* s);

bool s_end(stream* s);
char s_current_char(stream* s);
char s_next_char(stream* s);
void s_read_char(stream* s);

#endif
