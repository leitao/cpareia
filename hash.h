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
  pthread_mutex_t mutex;
#endif

} hash_t;

hash_t* hash_new();
void hash_free(hash_t*);
void hash_insert(hash_t*, char *, void *);
void hash_print(hash_t*);
void hash_foreach(hash_t *, GHFunc, void *);
size_t hash_size(hash_t *);

#endif
