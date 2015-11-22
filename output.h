#ifndef _OUTPUT_H_
#define _OUTPUT_H_

#include <stdlib.h>
#include <stdio.h>

typedef struct output_t {
  double min, max;
  char *filename;
} output_t;

output_t *output_new(char *, double, double);

void output_free(output_t *);
void output_print(output_t *);

#endif
