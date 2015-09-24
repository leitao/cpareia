#include "database.h"

database *database_new() {
  database *db;

  db = (database *) malloc(sizeof(database));

  db->size = 0;
  db->records = NULL;

  return db;
}

void database_free(database *db) {
  size_t i;

  for(i = 0; i < db->size; i++) {
    record_free(db->records[i]);
  }

  free(db->records);
  free(db);
}

record *database_remove_last_record(database *db) {
  record *rec;

  db->size--;

  rec = db->records[db->size];
  db->records = (record **) realloc(db->records, sizeof(record *) * (db->size));

  return rec;
}

void database_add_record(database *db, record *rec) {
  db->records = (record **) realloc(db->records, sizeof(record *) * (db->size + 1));
  db->records[db->size++] = rec;
}
