#ifndef _RECORD_H_
#define _RECORD_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>

#define MAX_BLOCK 9

typedef struct record_t {
  uint32_t _keys[MAX_BLOCK];
  uint32_t _num_fields;
  uint8_t *_indexes;
  char *_fields;
} record_t;

record_t *record_new(size_t, char *, uint8_t *);
void record_free(record_t *);
char *record_get_field(record_t *, uint8_t);
void record_print(record_t *);

#define record_get_field_size(record, i) (record->_indexes[(i)] - 1)
#define record_get_id(record) (record)->_fields

#endif
