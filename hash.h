#ifndef _HASH_H_
#define _HASH_H_

#include <glib.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "array.h"
#include "record.h"

typedef struct hash_t{
  GHashTable *table;
#ifndef SINGLE_BLOCKER
  GMutex mutex;
#endif
} hash_t;

hash_t* hash_new();
void hash_free(hash_t*);
void hash_insert(hash_t*, char *, void *);
void hash_print(hash_t*);

#endif
