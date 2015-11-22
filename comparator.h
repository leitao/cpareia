#ifndef _COMPARATOR_H_
#define _COMPARATOR_H_

#include "field_comparator.h"
#include "classifier.h"
#include "record.h"
#include "pool.h"
#include "project.h"
#include "output.h"

typedef double (*compare_fn)(char *, char *);

double compare_all(classifier_t *, record_t *, record_t *, double *);
void comparator_start(project_t *);
#endif
