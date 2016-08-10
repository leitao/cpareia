#ifndef _COMPARATOR_H_
#define _COMPARATOR_H_

#include <emmintrin.h>

#include "field_comparator.h"
#include "classifier.h"
#include "record.h"
#include "project.h"
#include "output.h"

#define between(val, min, max) val >= min && val <= max
#define MAX(a, b) a > b ? a : b
#define MIN(a, b) a < b ? a : b
#define TILE_SIDE 4

typedef struct comparator_pthread_params_t {
  project_t *project;
  int rank, num_threads;
} comparator_pthread_params_t;

pthread_t **comparator_run_async(project_t *);
#endif
