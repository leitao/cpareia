#ifndef _COMPARATOR_H_
#define _COMPARATOR_H_

#include "field_comparator.h"
#include "classifier.h"
#include "record.h"
#include "pool.h"
#include "project.h"
#include "output.h"

typedef double (*compare_fn)(char *, char *);

void comparator_start(project_t *, int);
#endif
