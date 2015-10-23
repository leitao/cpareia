#ifndef _CONJUNCTION_H_
#define _CONJUNCTION_H_

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "array.h"

typedef struct part {
  int field, size;
  char *transform;
} part;

typedef struct conjunction {
  array *parts;
} conjunction;

part *part_new(int, char *, int);
void part_free(part *);
void part_print(part *);
conjunction *conjunction_new(size_t);
void conjunction_free(conjunction *);
void conjunction_add_part(conjunction *, int, char *, int);
void conjunction_print(conjunction *);

#endif
