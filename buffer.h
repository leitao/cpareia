#ifndef _BUFFER_H_
#define _BUFFER_H_

#include <stdlib.h>
#include <stdio.h>

typedef struct buffer {
  char *begin;
  char *end;
} buffer;

char *buffer_to_string(buffer *);
void buffer_print(buffer *);

#endif
