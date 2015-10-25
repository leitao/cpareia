#include "database.h"

database *
database_new(int num_fields) {
  int i;
  database *db;

  db = (database *) malloc(sizeof(database));

  db->records = array_new();
  db->num_fields = num_fields;

  db->fields = (unsigned char **)
    malloc(sizeof(unsigned char *) * num_fields);

  for(i = 0; i < num_fields; i++) {
    db->fields[i] = NULL;
  }

  return db;
}

void
database_fini(database *db) {
  array_fini(db->records);
}

void
database_free(database *db) {
  size_t i;

  for(i = 0; i < array_size(db->records); i++) {
    record_free((record *) array_get(db->records, i));
  }

  for(i = 0; i < db->num_fields; i++) {
    free(db->fields[i]);
  }

  free(db->filename);
  free(db->fields);
  array_free(db->records);

  free(db);
}

void
database_read(database *db) {
  size_t i;
  csv *my_csv;
  csv_row *my_row;
  csv_fields *my_fields;
  record *rec;

  my_csv = csv_new(db->filename);
  my_row = csv_row_new(NULL, NULL);
  my_fields = csv_fields_new(db->num_fields);

  while(csv_get_row(my_csv, my_row)) {
    csv_row_get_fields(my_fields, my_row, db->sep);
    rec = record_new(db->num_fields);

    for(i = 0; i < db->num_fields; i++) {
      record_add_field(rec, my_fields->fields[i]);
    }

    array_append(db->records, (void *) rec);

  }
  database_fini(db);

  csv_fields_free(my_fields);
  csv_row_free(my_row);

  csv_free(my_csv);
}

void database_print(database *db) {
  size_t i;

  printf("File: %s\n", db->filename);
  printf("Number of Fields: %d\n", (int) db->num_fields);
  printf("Fields:\n");

  for(i = 0; i < db->num_fields; i++) {
    printf("%s\n", db->fields[i]);
  }

  for(i = 0; i < array_size(db->records); i++) {
    record_print(array_get(db->records, i));
  }
}
