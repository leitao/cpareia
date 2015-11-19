#ifndef _COMPARATOR_H_
#define _COMPARATOR_H_y

#include "field_comparator.h"
#include "classifier.h"
#include "record.h"
#include "project.h"
#include "pool.h"

typedef double (*compare_fn)(char *, char *);

double *compare_all(classifier_t *, record_t *, record_t *);
void comparator_start(project_t *);
#endif
