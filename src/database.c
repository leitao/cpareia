#include "database.h"

#ifdef HAVE_SSE2
int8_t
find_byte(char const *s, char ch) {
  unsigned u, v;
  __m128i x, zero, cx16;

  zero = _mm_setzero_si128();
  cx16 = _mm_set1_epi8(ch);

  x = _mm_loadu_si128((__m128i const *)s);
  u = _mm_movemask_epi8(_mm_cmpeq_epi8(zero, x));
  v = _mm_movemask_epi8(_mm_cmpeq_epi8(cx16, x)) & ~u & (u - 1);

  return (s + ffs(v) - 1) - s;
}
#else
int8_t
find_byte(char const *s, char ch) {
  int i;

  for(i = 0; i < 16; i++) {
    if(s[i] == ch)
      return i;
    if(!s[i])
      return 0;
  }
  return 0;
}
#endif

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
    record_free((record_t *) array_get(database->records, i));

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
  size_t total, rows, acc;
  uint16_t i;
  uint8_t *indexes, sep;
  int8_t where;
  char *record_begin, *begin, *bend, *end;
  double prop;
  record_t *record;

  acc = total = 0;
  rows = database->nrows;

  end = database->buf;
  bend = end + database->filesize / sizeof(*database->buf) - 1;

  sep = database->sep;

  while(end < bend) {
    record_begin = begin = end;

    indexes = malloc(sizeof(uint8_t) * database->nfields);

    for(i = 0; i < database->nfields - 1; i++) {
      while((where = find_byte(end, sep)) == -1)
        end += 16;

      end += where;

      *(end++) = '\0';

      indexes[i] = end - begin;
      begin = end;
    }

    while(*end != '\n')
      end++;

    *(end++) = '\0';
    indexes[i] = end - begin;

    record = record_new(database->nfields, record_begin, indexes);
    array_push(database->records, record);

    acc++;
    total++;

    if(acc == 1000000) {
      acc = 0;
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
