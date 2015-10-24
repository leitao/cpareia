#ifndef _CSV_H_
#define _CSV_H_

#include <stdlib.h>

#include "database.h"
#include "error.h"

typedef struct csv {
  size_t size, num_fields;
  char *buf;
  char *end;
  char *current;
} csv;

typedef struct csv_row {
  char *begin;
  char *end;
} csv_row;

typedef struct csv_fields {
  char **fields;
  size_t size;
} csv_fields;

csv *csv_new(char *filename);
csv_row *csv_get_row(csv *my_csv);
csv_fields *csv_row_get_fields(csv_row *, char, size_t);
void csv_free(csv *);
void csv_row_free(csv_row *);
void csv_fields_free(csv_fields *);

#endif
