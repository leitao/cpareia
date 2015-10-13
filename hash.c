#include "hash.h"

hash *
hash_new() {
  hash *my_hash;

  my_hash = (hash *) malloc(sizeof(hash));

  my_hash->last_id = -1;
  my_hash->table = g_hash_table_new(g_str_hash, NULL);

  return my_hash;
}

void
hash_free(hash *my_hash) {
  g_hash_table_destroy(my_hash->table);
  free(my_hash);
}

int
hash_get_id(hash *my_hash, char *key) {
  gpointer id;

  if(!g_hash_table_lookup(my_hash->table, key)) {
    my_hash->last_id++;
    g_hash_table_insert(my_hash->table, key, GINT_TO_POINTER(my_hash->last_id));
  }

  id = g_hash_table_lookup(my_hash->table, key);

  return GPOINTER_TO_INT(id);
}
