#ifndef _DATABASE_H_
#define _DATABASE_H_

#include <assert.h>
#include <csv.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include "cpareia.h"
#include "record.h"

typedef struct database {
  record **records;
  size_t size;
} database;

database *database_new();
void database_add_record(database *, record *);
record *database_remove_last_record(database *);
void database_free(database *);
database *read_database(char *file_name);
void database_print(database *db);

#endif
