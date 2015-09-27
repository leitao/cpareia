#ifndef _RECORD_H_
#define _RECORD_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct record {
  int num_fields, _used_fields;
  char **fields;
} record;

record *record_new(int numb_fields);
void record_add_field(record *, char *);
void record_free(record *);
void record_print(record *);

#endif
