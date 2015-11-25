#ifndef _RESULT_H_
#define _RESULT_H_

#include <stdlib.h>
#include "record.h"

typedef struct result_t {
  char *id1, *id2;
  char status;
  double score, *scores;
  int num_scores;
} result_t;
#endif

result_t *result_new(char *, char *, char, double, double *, int);
void result_free(result_t *);
