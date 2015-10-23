#include "hash.h"

hash *
hash_new() {
  hash *my_hash;

  my_hash = (hash *) malloc(sizeof(hash));

  my_hash->last_id = -1;
  my_hash->table = g_hash_table_new(g_str_hash, g_str_equal);

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
  char *my_key;

  my_key = strdup(key);

  if(!g_hash_table_lookup(my_hash->table, my_key)) {
    my_hash->last_id++;
    g_hash_table_insert(
        my_hash->table,
        my_key,
        GINT_TO_POINTER(my_hash->last_id)
        );
  }

  id = g_hash_table_lookup(my_hash->table, my_key);

  return GPOINTER_TO_INT(id);
}

void
hash_print_pair(gpointer key, gpointer value, gpointer user_data) {
  (void) user_data;
  printf("  %s => %d\n",(gchar *) key, GPOINTER_TO_INT(value));
}

void
hash_print(hash *my_hash) {
  printf("{\n");
  g_hash_table_foreach(my_hash->table, (GHFunc) hash_print_pair, NULL);
  printf("}\n");
}
