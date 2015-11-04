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

typedef struct database {
  array *records;
  unsigned char **fields, sep;
  size_t num_fields;
  char *filename;
} database;

database *database_new(int num_fields, size_t num_rows);
void database_free(database *);
void database_read(database *);
void database_print(database *);
void database_fini(database *);

#endif
