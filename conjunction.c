#include "conjunction.h"

part *
part_new(char *field_name, char *transform, unsigned int size) {
  part *my_part;

  my_part = (part *) malloc(sizeof(part));
  my_part->field_name = strdup(field_name);
  my_part->transform = strdup(transform);
  my_part->size = size;

  return my_part;
}

void
part_free(part *my_part) {
  free(my_part->field_name);
  free(my_part->transform);
  free(my_part);
}

conjunction *
conjunction_new(unsigned int size) {
  conjunction *my_conj;

  my_conj = (conjunction *) malloc(sizeof(conjunction));
  my_conj->parts = array_new_prealloc(size);

  return my_conj;
}

void
conjunction_free(conjunction *my_conj) {
  size_t i;

  for(i = 0; i < my_conj->parts->size; i++) {
    part_free(my_conj->parts->data[i]);
  }

  array_free(my_conj->parts);
  free(my_conj);

}
