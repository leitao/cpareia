#ifndef _POOL_H_
#define _POOL_H_

#include <stdlib.h>
#include <glib.h>

typedef struct pool_t {
  GThreadPool *pool;
} pool_t;

pool_t *pool_new(int, void *, GFunc);
void pool_push(pool_t *, void *);
void *pool_get_user_data(pool_t *);
void pool_free(pool_t *);

#endif
