#include "pool.h"

pool_t *
pool_new(int num_threads, project_t *project, GFunc func) {
  pool_t *pool;

  pool = malloc(sizeof(pool_t));
  pool->pool = g_thread_pool_new(func, project, num_threads, TRUE, NULL);

  return pool;
}

void
pool_push(pool_t *pool, void *data) {

  g_thread_pool_push(pool->pool, data, NULL);
}

void
pool_free(pool_t *pool) {
  g_thread_pool_free(pool->pool, FALSE, TRUE);
  free(pool);
}
