#include "record.h"

record_t *
record_new(size_t num_fields, char *fields, uint8_t *sizes) {
  record_t *record;

  record = malloc(sizeof(record_t));

  record->_indexes = sizes;
  record->_fields = fields;
  record->_num_fields = num_fields;
  record->_keys = NULL;

  return record;
}

char *
record_get_field(record_t *record, uint8_t field) {
  uint32_t i;
  uint8_t acc = 0;

  for(i = 0; i < field; i++)
    acc += record->_indexes[i];

  return record->_fields + acc;
}

void
record_free(record_t *record) {
  free(record->_indexes);
  free(record->_keys);
  free(record);
}

void
record_print(record_t *record) {
  uint32_t i;

  printf("FIELDS:\n");

  for(i = 0; i < record->_num_fields; i++)
    printf("\t%d: %s\n", i, record_get_field(record, i));
}
