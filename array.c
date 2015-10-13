#include "array.h"

array *
array_new() {
  array *ary;

  ary = (array *) malloc(sizeof(array));

  ary->size = 0;
  ary->_total_size = INITIAL_SIZE;
  ary->data = (void **) malloc(sizeof(void *) * INITIAL_SIZE);

  return ary;
}

void
array_fini(array *ary) {
  ary->data = (void **) realloc(ary->data, sizeof(void *) * ary->size);
  ary->_total_size = ary->size;
}

void
array_free(array *ary) {
  free(ary->data);
  free(ary);
}

void *
array_remove_last(array *ary) {
  ary->size--;

  return ary->data[ary->size];
}

void
array_add(array *ary, void *el) {
  if(ary->size == ary->_total_size) {
    ary->_total_size *= 2;
    ary->data = (void **)
      realloc(ary->data, sizeof(void *) * ary->_total_size);
  }

  ary->data[ary->size++] = el;
}
