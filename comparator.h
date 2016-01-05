#ifndef _COMPARATOR_H_
#define _COMPARATOR_H_

#include "field_comparator.h"
#include "classifier.h"
#include "record.h"
#include "pool.h"
#include "project.h"
#include "output.h"

#define between(val, min, max) val >= min && val <= max

typedef struct work_t {
  array_t *array;
  int start, step;
} work_t;

work_t *work_new(array_t *, int, int);
void work_free(work_t *);

void comparator_run(project_t *, int);
#endif
