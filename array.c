#include "array.h"

array *
array_new() {
  return array_new_prealloc(INITIAL_SIZE);
}

array *
array_new_prealloc(size_t size) {
  array *ary;

  ary = malloc(sizeof(array));

  ary->_size = 0;
  ary->_total_size = size;
  ary->_data = malloc(sizeof(void *) * size);

  return ary;
}

void
array_fini(array *ary) {
  ary->_data = realloc(ary->_data, sizeof(void *) * ary->_size);
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
array_realloc(array *ary) {
  ary->_total_size *= 2;
  ary->_data = (void **)
    realloc(ary->_data, sizeof(void *) * ary->_total_size);
}

void
array_add_at(array *ary, void *el, size_t i) {
  if(ary->_size == ary->_total_size) {
    array_realloc(ary);
  }
  ary->_data[i] = el;
  ary->_size++;
}

void
array_append(array *ary, void *el) {
  array_add_at(ary, el, ary->_size);
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

void
array_print(array *ary, array_fn_print fn_print) {
  size_t i;

  for(i = 0; i < array_size(ary); i++) {
    fn_print(array_get(ary, i));
  }
}
