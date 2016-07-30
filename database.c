#include "database.h"

void
database_open_file(database_t *database) {
  int fd;
  struct stat fs;

  fd = open(database->filename, O_RDONLY);

  if (fd == -1)
    handle_error("Unable to open file %s\n", database->filename);

  if (fstat(fd, &fs) == -1)
    handle_error("fstat\n");

  database->buf = mmap(NULL, fs.st_size, PROT_WRITE | PROT_READ, MAP_PRIVATE, fd, 0);

  if (database->buf == MAP_FAILED)
    handle_error("mmap\n");

  close(fd);

  database->filesize = fs.st_size;
}

database_t *
database_new(char *filename, int nfields, size_t nrows, char sep) {
  int i;
  database_t *database;

  database = malloc(sizeof(database_t));

  database->records = array_new_zeroed(nrows);
  database->nfields = nfields;
  database->nrows = nrows;
  database->filename = filename;
  database->sep = sep;

  database->fields = malloc(sizeof(unsigned char *) * nfields);

  for(i = 0; i < nfields; i++)
    database->fields[i] = NULL;

  database_open_file(database);

  return database;
}

void
database_free(database_t *database) {
  size_t i;

  if(!database)
    return;

  for(i = 0; i < array_size(database->records); i++)
    record_shallow_free((record_t *) array_get(database->records, i));

  for(i = 0; i < database->nfields; i++)
    free(database->fields[i]);

  free(database->filename);
  free(database->fields);
  array_free(database->records);

  munmap(database->buf, database->filesize);

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
  size_t i, total, rows;
  uint8_t *indexes;
  char sep, *record_begin, *begin, *end, *bend;
  double prop;
  record_t *record;

  total = 0;
  rows = database->nrows;
  sep = database->sep;

  end = database->buf;
  bend = database->buf + database->filesize / sizeof(*database->buf) - 1;

  while(end < bend) {
    record_begin = begin = end;
    i = 0;

    indexes = malloc(sizeof(uint8_t) * database->nfields);

    while(*end != '\n') {
      if(*end == sep) {
        indexes[i++] = end - begin + 1;
        *end = '\0';
        begin = end + 1;
      }
      end++;
    }
    *(end++) = '\0';

    /* Skipping multiple \n's */
    while(*end == '\n' && end != bend)
      end++;

    record = record_new_full(database->nfields, record_begin, indexes);
    array_push(database->records, record);

    total++;

    if(!(total % 1000000)) {
      prop = 100.0 * total / rows;
      printf("Registros lidos: %lu/%lu (%2.2f%%)\n", total, rows, prop);
    }
  }

  printf("Registros lidos: %lu/%lu (%2.2f%%)\n", rows, rows, 100.0);
}

void
database_print(database_t *database) {
  size_t i;

  printf("  File: %s\n", database->filename);
  printf("  Number of Fields: %d\n", (int) database->nfields);
  printf("  Fields:\n");

  for(i = 0; i < database->nfields; i++)
    printf("    %s\n", database->fields[i]);

  for(i = 0; i < array_size(database->records); i++)
    record_print(array_get(database->records, i));
}
