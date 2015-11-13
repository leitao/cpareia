#include "blocking.h"

void
blocking_generate_keys(project_t *project, record_t *record) {
  size_t i, j;
  conjunction_t *conjunction;
  part_t *part;
  char buffer[5], key[1024];

  for(i = 0; i < array_size(project->conjunctions); i++) {
    conjunction = array_get(project->conjunctions, i);
    key[0] = '\0';

    for(j = 0; j < array_size(conjunction->parts); j++) {
      part = array_get(conjunction->parts, j);

      if(!part->transform) {
        strcat(key, record->fields[part->field]);
      } else if(!strcmp(part->transform, "brsoundex")) {
        brsoundex(record->fields[part->field], buffer, 5);
        strcat(key, buffer);
      }
      else {
        handle_error("Unknown transformation");
      }
    }
    if(strlen(key)) {
      hash_insert(project->blocks, key, record);
    }
  }
}

void
blocking_print(project_t *project) {
  hash_print(project->blocks);
}
