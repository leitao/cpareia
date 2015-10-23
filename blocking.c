#include "blocking.h"

void
blocking_generate_keys(project *my_proj, size_t id, hash *my_hash, int *ids) {
  size_t i, j;
  record *rec;
  conjunction *my_conj;
  part *my_part;
  char buffer[5], key[1024];

  rec = array_get(my_proj->d0->records, id);

  for(i = 0; i < array_size(my_proj->conjunctions); i++) {
    my_conj = array_get(my_proj->conjunctions, i);
    key[0] = '\0';

    ids[i] = -1;

    for(j = 0; j < array_size(my_conj->parts); j++) {
      my_part = array_get(my_conj->parts, j);

      if(!my_part->transform) {
        strcat(key, rec->fields[my_part->field]);
      } else if(!strcmp(my_part->transform, "brsoundex")) {
        brsoundex(rec->fields[my_part->field], buffer, 5);
        strcat(key, buffer);
      }
      else {
        handle_error("Unknown transformation");
      }
    }
    if(strlen(key)) {
      ids[i] = hash_get_id(my_hash, key);
    }
  }
}

void
blocking_generate(project *my_proj) {
  size_t i, j;
  size_t conj_size;
  hash *my_hash;
  int *ids;

  my_hash = hash_new();
  conj_size = array_size(my_proj->conjunctions);
  ids = (int *)malloc(sizeof(int) * conj_size);

  for(i = 0; i < array_size(my_proj->d0->records); i++) {
    blocking_generate_keys(my_proj, i, my_hash, ids);

    for(j = 0; j < conj_size; j++) {
      if(ids[j] != -1) {
        printf("Blocagem: %d, conj: %d\n", ids[j], (int) j);
      }
    }
  }

  free(ids);
  hash_free(my_hash);
}
