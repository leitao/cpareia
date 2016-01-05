#include "pool.h"

pool_t *
pool_new(int num_threads, void *data, GFunc func) {
  pool_t *pool;

  pool = malloc(sizeof(pool_t));
  pool->pool = g_thread_pool_new(func, data, num_threads, TRUE, NULL);

  return pool;
}

void
pool_push(pool_t *pool, void *data) {
  g_thread_pool_push(pool->pool, data, NULL);
}

void *
pool_get_user_data(pool_t *pool) {
  return pool->pool->user_data;
}

void
pool_free(pool_t *pool) {
  g_thread_pool_free(pool->pool, FALSE, TRUE);
  free(pool);
}
