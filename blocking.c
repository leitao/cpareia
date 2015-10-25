#include "blocking.h"

void
blocking_generate_keys(project *my_proj, record *rec) {
  size_t i, j;
  conjunction *my_conj;
  part *my_part;
  char buffer[5], key[1024];

  for(i = 0; i < array_size(my_proj->conjunctions); i++) {
    my_conj = array_get(my_proj->conjunctions, i);
    key[0] = '\0';

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
      hash_insert(my_proj->blocks, key, rec);
    }
  }
}

void
blocking_generate(project *my_proj) {
  size_t i;

  for(i = 0; i < array_size(my_proj->d0->records); i++) {
    blocking_generate_keys(my_proj, array_get(my_proj->d0->records, i));
  }
}

void
blocking_print(project *my_proj) {
  hash_print(my_proj->blocks);
}
