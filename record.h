#ifndef _RECORD_H_
#define _RECORD_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct record_t {
  int num_fields, _used_fields;
  char **fields;
} record_t;

record_t *record_new(int numb_fields);
void record_add_field(record_t *, char *);
void record_free(record_t *);
void record_print(record_t *);
char *record_get_field(record_t *, int);

#endif
