#ifndef _DATABASE_H_
#define _DATABASE_H_

#include <assert.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include "record.h"
#include "errors.h"
#include "array.h"
#include "csv.h"

typedef struct database_t {
  array_t *records;
  unsigned char **fields, sep;
  size_t num_fields;
  char *filename;
} database_t;

typedef void (*database_read_callback)(record_t *);

database_t *database_new(int num_fields, size_t num_rows);
void database_free(database_t *);
void database_read(database_t *);
void database_read_with_callback(database_t *, database_read_callback);
void database_print(database_t *);
void database_fini(database_t *);

#endif
