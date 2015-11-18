#ifndef _CLASSIFIER_H_
#define _CLASSIFIER_H_

#include <string.h>
#include <stdio.h>

#include "array.h"
#include "colors.h"

typedef struct comparator_t {
  int exact, use_weight_table, field1;
  double u, m, missing, min_value_to_be_match;
  char *frequency_table, *function;
} comparator_t;

typedef struct classifier_t {
  int match_min;
  int not_match_max;
  array_t *comparators;
} classifier_t;

comparator_t *
comparator_new(int, int, double, double, double, int, char *, char *, double);
void comparator_free(comparator_t *comparator);
classifier_t *classifier_new(int, int);
void classifier_free(classifier_t *);
void classifier_print(classifier_t *);

#endif
