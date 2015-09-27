#include "record.h"
#include <assert.h>

record *record_new(int num_fields) {
  record *rec;

  rec = (record *) malloc(sizeof(record));
  rec->num_fields = num_fields;
  rec->_used_fields = 0;

  rec->fields = (char **) malloc(sizeof(char *) * num_fields);

  return rec;
}

void record_add_field(record *rec, char *field) {
  assert(rec->_used_fields < rec->num_fields);
  rec->fields[rec->_used_fields++] = field;
}

void record_free(record *rec) {
  int i;

  for(i = 0; i < rec->_used_fields; i++) {
    free(rec->fields[i]);
  }
  free(rec->fields);
  free(rec);
}

void record_print(record *rec) {
  int i;

  printf("FIELDS:\n");

  for(i = 0; i < rec->_used_fields; i++) {
    printf("\t%d: %s\n", i, rec->fields[i]);
  }
}
