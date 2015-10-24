#ifndef _CSV_H_
#define _CSV_H_

#include <stdlib.h>

#include "database.h"
#include "error.h"

typedef struct csv {
  size_t size;
  char *buf;
  char *end;
  char *current;
  char sep;
} csv;

typedef struct csv_row {
  char *begin;
  char *end;
} csv_row;

typedef struct csv_fields {
  char **fields;
  size_t size;
} csv_fields;

#endif
