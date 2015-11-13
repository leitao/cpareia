#ifndef _BLOCKING_H_
#define _BLOCKING_H_

#include "array.h"
#include "record.h"
#include "project.h"
#include "field_comparator.h"
#include "hash.h"

void blocking_generate_keys(project_t *, record_t *);
void blocking_print(project_t *);

#endif
