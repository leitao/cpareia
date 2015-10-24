#include "array.h"

array *
array_new() {
  return array_new_prealloc(INITIAL_SIZE);
}

array *
array_new_prealloc(size_t size) {
  array *ary;

  ary = (array *) malloc(sizeof(array));

  ary->_size = 0;
  ary->_total_size = size;
  ary->_data = (void **) malloc(sizeof(void *) * size);

  return ary;
}

void
array_fini(array *ary) {
  ary->_data = (void **) realloc(ary->_data, sizeof(void *) * ary->_size);
  ary->_total_size = ary->_size;
}

void
array_free(array *ary) {
  free(ary->_data);
  free(ary);
}

void *
array_remove_last(array *ary) {
  ary->_size--;

  return ary->_data[ary->_size];
}

void
array_add(array *ary, void *el) {
  if(ary->_size == ary->_total_size) {
    ary->_total_size *= 2;
    ary->_data = (void **)
      realloc(ary->_data, sizeof(void *) * ary->_total_size);
  }

  ary->_data[ary->_size++] = el;
}

inline void *
array_get(array *ary, size_t i) {
  return i < ary->_size ? ary->_data[i] : NULL;
}

inline void *
array_get_last(array *ary) {
  return ary->_data[ary->_size -1];
}

inline size_t
array_size(array *ary) {
  return ary->_size;
}
