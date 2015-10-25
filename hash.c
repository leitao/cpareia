#include "hash.h"

hash *
hash_new() {
  hash *my_hash;

  my_hash = (hash *) malloc(sizeof(hash));
  my_hash->table = g_hash_table_new_full(
      g_str_hash,
      g_str_equal,
      (GDestroyNotify) free,
      (GDestroyNotify) array_free);

  return my_hash;
}

void
hash_free(hash *my_hash) {
  g_hash_table_destroy(my_hash->table);
  free(my_hash);
}

void
hash_insert(hash *my_hash, char *key, void *rec) {
  array *my_array;

  if(!g_hash_table_lookup(my_hash->table, key)) {
    g_hash_table_insert(
        my_hash->table,
        strdup(key),
        array_new()
        );
  }

  my_array = (array *) g_hash_table_lookup(my_hash->table, key);
  array_append(my_array, rec);
}

void
record_void_print(void *rec) {
  record_print((record *) rec);
}

void
hash_print_pair(gpointer key, gpointer value, gpointer data) {
  (void) data;
  printf("  %s =>\n",(gchar *) key);
  array_print(value, record_void_print);
}

void
hash_print(hash *my_hash) {
  printf("{\n");
  g_hash_table_foreach(my_hash->table, (GHFunc) hash_print_pair, NULL);
  printf("}\n");
}
