#include "conjunction.h"

part *
part_new(char *field_name, char *transform, int size) {
  part *my_part;

  my_part = (part *) malloc(sizeof(part));
  my_part->field_name = field_name ? strdup(field_name) : NULL;
  my_part->transform = transform ? strdup(transform) : NULL;
  my_part->size = size;

  return my_part;
}

void
part_print(part *my_part) {
  printf("part:\n");
  printf("field_name: %s\n", my_part->field_name);
  printf("transform: %s\n", my_part->transform);
  printf("size: %d\n", my_part->size);
}

void
part_free(part *my_part) {
  free(my_part->field_name);
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
    char *field_name,
    char *transform,
    int size) {
  part *my_part;
  my_part = part_new(field_name, transform, size);

  array_add(my_conj->parts, my_part);
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

void
conjunction_print(conjunction *my_conj) {
  size_t i;

  for(i = 0; i < my_conj->parts->size; i++) {
    part_print(my_conj->parts->data[i]);
  }
}
