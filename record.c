#include "record.h"
#include <assert.h>

record_t *
record_new(int num_fields) {
  record_t *record;

  record = malloc(sizeof(record_t));
  record->num_fields = num_fields;
  record->_used_fields = 0;
  record->fields = malloc(sizeof(char *) * num_fields);

  return record;
}

void
record_add_field(record_t *record, char *field) {
  assert(record->_used_fields < record->num_fields);
  record->fields[record->_used_fields++] = field;
}

void
record_free(record_t *record) {
  int i;

  for(i = 0; i < record->_used_fields; i++) {
    free(record->fields[i]);
  }
  free(record->fields);
  free(record);
}

void
record_print(record_t *record) {
  int i;

  printf("FIELDS:\n");

  for(i = 0; i < record->_used_fields; i++) {
    printf("\t%d: %s\n", i, record->fields[i]);
  }
}

char *
record_get_field(record_t *record, int field) {
  return record->fields[field];
}
