#ifndef _OUTPUT_H_
#define _OUTPUT_H_

#include <stdlib.h>
#include <stdio.h>
#include "record.h"
#include "pool.h"
#include "errors.h"
#include "result.h"

typedef struct output_t {
  double min, max;
  char *filename;
  FILE *file;
  pool_t *pool;
} output_t;

output_t *output_new(char *, double, double);

void output_free(output_t *);
void output_print(output_t *);
void output_push(output_t *, result_t *);

#endif
