#ifndef _DATABASE_H_
#define _DATABASE_H_

#include <stdlib.h>
#include "record.h"

typedef struct database {
  record **records;
  size_t size;
} database;

database *database_new();
void database_add_record(database *, record *);

#endif
