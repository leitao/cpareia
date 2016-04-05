#include "database.h"

database_t *
database_new(int num_fields, size_t num_rows) {
  int i;
  database_t *database;

  database = malloc(sizeof(database_t));

  database->records = calloc(sizeof(record_t), num_rows);
  database->num_rows = num_rows;
  database->num_fields = num_fields;

  database->fields = malloc(sizeof(unsigned char *) * num_fields);

  for(i = 0; i < num_fields; i++) {
    database->fields[i] = NULL;
  }

  return database;
}

void
database_free(database_t *database) {
  size_t i;

  if(!database) {
    return;
  }
  for(i = 0; i < database->num_rows; i++) {
    record_shallow_free(&database->records[i]);
  }

  for(i = 0; i < database->num_fields; i++) {
    free(database->fields[i]);
  }

  free(database->filename);
  free(database->fields);
  free(database->records);

  free(database);
}
void *
database_read_simple(void *database) {
  database_read(database);

  return NULL;
}

pthread_t *
database_read_async(database_t *database) {
  pthread_t *thread;

  thread = malloc(sizeof(pthread_t));

  pthread_create(thread, NULL, database_read_simple, database);

  return thread;
}

void
database_read(database_t *database) {
  size_t i, total;
  csv_t *csv;
  csv_row_t *csv_row;
  csv_fields_t *csv_fields;
  record_t *record;

  csv = csv_new(database->filename);
  csv_row = csv_row_new(NULL, NULL);

  total = 0;

  while(csv_get_row(csv, csv_row)) {
    record = &database->records[total];
    record_init(record, database->num_fields);

    csv_fields = csv_fields_new(database->num_fields);
    csv_get_fields(csv_fields, csv_row, database->sep);

    for(i = 0; i < database->num_fields; i++) {
      record_add_field(record, csv_fields->fields[i]);
    }

    csv_fields_deep_free(csv_fields);

    total++;

    if(!(total % 1000000)) {
      printf(
          "Registros lidos: %d de %d (%2.2f%%)\n",
          (int) total,
          (int) database->num_rows,
          100.0 * total / database->num_rows
          );
    }
  }

  printf(
      "Registros lidos: %d de %d (%2.2f%%)\n",
      (int) total,
      (int) database->num_rows,
      100.0 * total / database->num_rows
      );

  csv_row_free(csv_row);

  csv_free(csv);
}

void
database_print(database_t *database) {
  size_t i;

  printf("  File: %s\n", database->filename);
  printf("  Number of Fields: %d\n", (int) database->num_fields);
  printf("  Fields:\n");

  for(i = 0; i < database->num_fields; i++) {
    printf("    %s\n", database->fields[i]);
  }

  for(i = 0; i < database->num_rows; i++) {
    record_print(&database->records[i]);
  }
}
