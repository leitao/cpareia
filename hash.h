#ifndef _HASH_H_
#define _HASH_H_

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "khash.h"
#include "array.h"
#include "record.h"

KHASH_MAP_INIT_STR(str, array_t *)

typedef struct hash_t{
  khash_t(str) *table;

  pthread_mutex_t mutex;

} hash_t;

hash_t* hash_new();
/*
void hash_free(hash_t*);
void hash_insert(hash_t*, char *, void *);
void hash_print(hash_t*);
void hash_foreach(hash_t *, GHFunc, void *);
void hash_foreach_remove(hash_t *, GHRFunc, void *);
size_t hash_size(hash_t *);
void *hash_get(hash_t *, char *);
*/

#endif
