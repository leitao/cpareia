#include "record.h"
#include <assert.h>

record *record_new(int id) {
  record *rec;

  rec = (record *) malloc(sizeof(record));
  rec->id = id;
  rec->num_fields = 0;
  rec->fields = NULL;

  return rec;
}

void record_add_field(record *rec, char *field) {
  if (rec->num_fields) {
    rec->fields = (char **) realloc(rec->fields, sizeof(char *) * (rec->num_fields + 1));
  } else {
    rec->fields = (char **) malloc(sizeof(char *));
  }
  rec->fields[rec->num_fields++] = field;
}

void record_free(record *rec) {
  int i;

  for(i = 0; i < rec->num_fields; i++) {
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
