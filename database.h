#ifndef _DATABASE_H_
#define _DATABASE_H_

#include <assert.h>
#include <csv.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include "record.h"
#include "errors.h"

#define INITIAL_SIZE 10000

typedef struct database {
  record **records;
  unsigned char **fields, sep;
  size_t size, _total_size, num_fields;
  unsigned char *filename;
} database;

database *database_new(int num_fields);
void database_add_record(database *, record *);
record *database_remove_last_record(database *);
void database_free(database *);
void database_read(database *);
void database_print(database *);
void database_fini(database *);

#endif
