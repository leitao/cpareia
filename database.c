#include "database.h"

database *
database_new(int num_fields) {
  int i;
  database *db;

  db = (database *) malloc(sizeof(database));

  db->size = 0;
  db->_total_size = INITIAL_SIZE;
  db->records = (record **) malloc(sizeof(record *) * INITIAL_SIZE);
  db->num_fields = num_fields;

  db->fields = (unsigned char **) malloc(sizeof(unsigned char *) * num_fields);

  for(i = 0; i < num_fields; i++) {
    db->fields[i] = NULL;
  }

  return db;
}

void
database_free(database *db) {
  size_t i;

  for(i = 0; i < db->size; i++) {
    record_free(db->records[i]);
  }

  for(i = 0; i < db->num_fields; i++) {
    free(db->fields[i]);
  }

  free(db->filename);
  free(db->fields);
  free(db->records);

  free(db);
}

record *
database_remove_last_record(database *db) {
  record *rec;

  db->size--;

  rec = db->records[db->size];

  return rec;
}

void
database_add_record(database *db, record *rec) {
  if(db->size == db->_total_size) {
    db->records = (record **) realloc(db->records, sizeof(record *) * (db->size * 2));
    db->_total_size *= 2;
  }

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
  database_add_record((database *) db, record_new(((database *)db)->num_fields));
}

void
database_read(database *db) {
  size_t size;
  size_t i;
  char *buf;
  struct csv_parser p;
  record *rec;

  csv_init(&p, CSV_APPEND_NULL);
  csv_set_delim(&p, db->sep);

  size = open_file((char *) db->filename, &buf);

  database_add_record(db, record_new(db->num_fields));

  i = csv_parse(&p, buf, size, new_field, new_record, (void *) db);

  assert(i == size);

  csv_fini(&p, new_field, new_record, NULL);
  csv_free(&p);

  if (db->records[db->size - 1]->num_fields == 0) {
    rec = database_remove_last_record(db);
    record_free(rec);
  }
}

void database_print(database *db) {
  size_t i;

  printf("File: %s\n", db->filename);
  printf("Number of Fields: %d\n", (int) db->num_fields);
  printf("Fields:\n");

  for(i = 0; i < db->num_fields; i++) {
    printf("%s\n", db->fields[i]);
  }

  for(i = 0; i < db->size; i++) {
    record_print(db->records[i]);
  }
}
