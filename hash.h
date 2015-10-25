#ifndef _HASH_H_
#define _HASH_H_

#include <glib.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "array.h"
#include "record.h"

typedef struct hash {
  GHashTable *table;
} hash;

hash * hash_new();
void hash_free(hash *);
void hash_insert(hash *, char *, void *);
void hash_print(hash *);

#endif
