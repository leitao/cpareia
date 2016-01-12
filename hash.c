#include "hash.h"

hash_t *
hash_new() {
  hash_t *hash;

  hash = malloc(sizeof(hash_t));

  hash->table = kh_init(str);

  pthread_mutex_init(&hash->mutex, NULL);

  return hash;
}

void
hash_free(hash_t *hash) {
  kh_destroy(str, hash->table);

  pthread_mutex_destroy(&hash->mutex);

  free(hash);
}

void
hash_insert(hash_t *hash, char *key, void *record) {
  int absent;
  array_t *array;
  khint_t k;

  pthread_mutex_lock(&hash->mutex);

  k = kh_put(str, hash->table, key, &absent);

  if (absent) {
    kh_key(hash->table, k) = strdup(key);
  }

  if(k == kh_end(hash->table)) {
    kh_value(hash->table, k) = array_new(1);
  }

  k = kh_get(str, hash->table, key);

  array = kh_val(hash->table, k);

  array_append(array, record);

  pthread_mutex_unlock(&hash->mutex);
}
/*

void
record_void_print(void *record) {
  record_print(record);
}
*/

array_t *
hash_get(hash_t *hash, char *key) {
  khint_t k;

  k = kh_get(str, hash->table, key);
  return kh_val(hash->table, k);
}

/*

void
hash_print_pair(gpointer key, gpointer value, gpointer data) {
  (void) data;
  printf("  %s =>\n",(gchar *) key);
  array_print(value, record_void_print);
}

void
hash_foreach_remove(hash_t *hash, GHRFunc fn, void *data) {
  g_hash_table_foreach_remove(hash->table, fn, data);
}

void
hash_foreach(hash_t *hash, GHFunc fn, void *data) {
  g_hash_table_foreach(hash->table, fn, data);
}
*/
size_t
hash_size(hash_t *hash) {
  return kh_size(hash->table);
}
/*

void
hash_print(hash_t *hash) {
  printf("{\n");
  hash_foreach(hash, hash_print_pair, NULL);
  printf("}\n");
}
*/
