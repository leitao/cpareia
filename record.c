#include "record.h"
#include <assert.h>

record *record_new(int id, int num_fields) {
  record *rec;

  rec = (record *) malloc(sizeof(record));
  rec->id = id;
  rec->num_fields = num_fields;
  rec->_current_fields = 0;
  rec->fields = (char **) malloc(sizeof(char *) * num_fields);

  return rec;
}

void record_add_field(record *rec, char *field) {
  assert(rec->_current_fields < rec->num_fields);

  rec->fields[rec->_current_fields] = field;
  rec->_current_fields++;
}

void record_free(record *rec) {
  int i;

  for(i = 0; i < rec->_current_fields; i++) {
    free(rec->fields[i]);
  }
  free(rec->fields);
  free(rec);
}

void record_print(record *rec) {
  int i;

  printf("ID=%d\n", rec->id);
  printf("NUM_FIELDS=%d\n", rec->num_fields);
  printf("FIELDS:\n");

  for(i = 0; i < rec->num_fields; i++) {
    printf("\t%d: %s\n", i, rec->fields[i]);
  }
}
