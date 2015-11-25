#ifndef _RESULT_H_
#define _RESULT_H_

#include <stdlib.h>
#include "record.h"

typedef struct result_t {
  record_t *r1, *r2;
  char status;
  double score, *scores;
  int num_scores;
} result_t;
#endif

result_t *result_new(record_t *, record_t *, char, double, double *, int);
void result_free(result_t *);
