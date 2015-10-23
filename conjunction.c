#include "conjunction.h"

part *
part_new(int field, char *transform, int size) {
  part *my_part;

  my_part = (part *) malloc(sizeof(part));
  my_part->field = field;
  my_part->transform = transform ? strdup(transform) : NULL;
  my_part->size = size;

  return my_part;
}

void
part_print(part *my_part) {
  printf("part:\n");
  printf("field: %d\n", my_part->field);
  printf("transform: %s\n", my_part->transform);
  printf("size: %d\n", my_part->size);
}

void
part_free(part *my_part) {
  free(my_part->transform);
  free(my_part);
}

conjunction *
conjunction_new(size_t size) {
  conjunction *my_conj;

  my_conj = (conjunction *) malloc(sizeof(conjunction));
  my_conj->parts = array_new_prealloc(size);

  return my_conj;
}

void
conjunction_add_part(
    conjunction *my_conj,
    int field,
    char *transform,
    int size) {
  part *my_part;
  my_part = part_new(field, transform, size);

  array_add(my_conj->parts, my_part);
}

void
conjunction_free(conjunction *my_conj) {
  size_t i;

  for(i = 0; i < array_size(my_conj->parts); i++) {
    part_free(array_get(my_conj->parts, i));
  }

  array_free(my_conj->parts);
  free(my_conj);
}

void
conjunction_print(conjunction *my_conj) {
  size_t i;

  for(i = 0; i < array_size(my_conj->parts); i++) {
    part_print(array_get(my_conj->parts, i));
  }
}
