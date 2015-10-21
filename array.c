#include "array.h"

array *
array_new() {
  return array_new_prealloc(INITIAL_SIZE);
}

array *
array_new_prealloc(size_t size) {
  array *ary;

  ary = (array *) malloc(sizeof(array));

  ary->size = 0;
  ary->_total_size = size;
  ary->_data = (void **) malloc(sizeof(void *) * size);

  return ary;
}

void
array_fini(array *ary) {
  ary->_data = (void **) realloc(ary->_data, sizeof(void *) * ary->size);
  ary->_total_size = ary->size;
}

void
array_free(array *ary) {
  free(ary->_data);
  free(ary);
}

void *
array_remove_last(array *ary) {
  ary->size--;

  return ary->_data[ary->size];
}

void
array_add(array *ary, void *el) {
  if(ary->size == ary->_total_size) {
    ary->_total_size *= 2;
    ary->_data = (void **)
      realloc(ary->_data, sizeof(void *) * ary->_total_size);
  }

  ary->_data[ary->size++] = el;
}

inline void *
array_get(array *ary, size_t i) {
  return i < ary->size ? ary->_data[i] : NULL;
}
