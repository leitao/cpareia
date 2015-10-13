#ifndef _CONJUNCTION_H_
#define _CONJUNCTION_H_

#include <string.h>
#include <stdlib.h>
#include "array.h"

typedef struct part {
  char *field_name, *transform;
  unsigned int size;
} part;

typedef struct conjunction {
  array *parts;
} conjunction;

#endif
