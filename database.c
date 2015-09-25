#include "database.h"

database *
database_new() {
  database *db;

  db = (database *) malloc(sizeof(database));

  db->size = 0;
  db->records = NULL;

  return db;
}

void
database_free(database *db) {
  size_t i;

  for(i = 0; i < db->size; i++) {
    record_free(db->records[i]);
  }

  free(db->records);
  free(db);
}

record *
database_remove_last_record(database *db) {
  record *rec;

  db->size--;

  rec = db->records[db->size];
  db->records = (record **) realloc(db->records, sizeof(record *) * (db->size));

  return rec;
}

void
database_add_record(database *db, record *rec) {
  db->records = (record **) realloc(db->records, sizeof(record *) * (db->size + 1));
  db->records[db->size++] = rec;
}

int
open_file(char *fname, char **buf) {
  int fd;
  struct stat fs;

	fd = open(fname, O_RDONLY);

	if (fd == -1)
		handle_error("open");

  if (fstat(fd, &fs) == -1)
    handle_error("fstat");

	*buf = mmap(NULL, fs.st_size, PROT_READ, MAP_PRIVATE, fd, 0);

  if (*buf == MAP_FAILED)
    handle_error("mmap");

  close(fd);

  return fs.st_size;
}

void
new_field(void *parsed, size_t size, void *db) {
  char *string;
  database *my_db;
  record *rec;

  my_db = (database *) db;
  rec = my_db->records[my_db->size - 1];

  string = (char *)malloc(sizeof(char) * (size + 1));
  strcpy(string, (char *) parsed);

  record_add_field(rec, string);
}

void
new_record(int c, void *db) {
  assert(c == '\n');
  database_add_record((database *) db, record_new(((database *)db)->size));
}

database *
read_database(char *file_name) {
  size_t size;
  size_t i;
  char *buf;
  database *db;
  struct csv_parser p;
  record *rec;

  db = database_new();

  csv_init(&p, CSV_APPEND_NULL);
  csv_set_delim(&p, '\t');

  size = open_file(file_name, &buf);

  database_add_record(db, record_new(0));

  i = csv_parse(&p, buf, size, new_field, new_record, (void *) db);

  assert(i == size);

  csv_fini(&p, new_field, new_record, NULL);
  csv_free(&p);

  if (db->records[db->size - 1]->num_fields == 0) {
    rec = database_remove_last_record(db);
    record_free(rec);
  }

  return db;
}

void database_print(database *db) {
  size_t i;

  for(i = 0; i < db->size; i++) {
    record_print(db->records[i]);
  }
}
