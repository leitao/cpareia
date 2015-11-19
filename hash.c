#include "hash.h"

hash_t *
hash_new() {
  hash_t *hash;

  hash = malloc(sizeof(hash_t));
  hash->table = g_hash_table_new_full(
      g_str_hash,
      g_str_equal,
      (GDestroyNotify) free,
      (GDestroyNotify) array_free);

#ifndef SINGLE_BLOCKER
  g_mutex_init(&hash->mutex);
#endif

  return hash;
}

void
hash_free(hash_t *hash) {
  g_hash_table_destroy(hash->table);

#ifndef SINGLE_BLOCKER
  g_mutex_clear(&hash->mutex);
#endif

  free(hash);
}

void
hash_insert(hash_t *hash, char *key, void *record) {
  array_t *array;

#ifndef SINGLE_BLOCKER
  g_mutex_lock(&hash->mutex);
#endif

  if(!(array = g_hash_table_lookup(hash->table, key))) {
    array = array_new_prealloc(1);
    g_hash_table_insert(hash->table, strdup(key), array);
  }

  array_append(array, record);

#ifndef SINGLE_BLOCKER
  g_mutex_unlock(&hash->mutex);
#endif
}

void
record_void_print(void *record) {
  record_print(record);
}

void
hash_print_pair(gpointer key, gpointer value, gpointer data) {
  (void) data;
  printf("  %s =>\n",(gchar *) key);
  array_print(value, record_void_print);
}

void
hash_foreach(hash_t *hash, GHFunc fn, void *data) {
  g_hash_table_foreach(hash->table, fn, data);
}

void
hash_print(hash_t *hash) {
  printf("{\n");
  hash_foreach(hash, hash_print_pair, NULL);
  printf("}\n");
}
