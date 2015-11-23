#include "array.h"

array_t *
array_new_zeroed(size_t size) {
  array_t *array;

  array = array_new(size);
  bzero(array->_data, sizeof(void *) * size);

  return array;
}

array_t *
array_new(size_t size) {
  array_t *array;

  array = malloc(sizeof(array_t));

  array->_size = 0;
  array->_total_size = size;
  array->_data = malloc(sizeof(void *) * size);

  return array;
}

void
array_free(array_t *array) {
  free(array->_data);
  free(array);
}

void
array_realloc(array_t *array) {
  array->_total_size *= 2;
  array->_data =
    realloc(array->_data, sizeof(void *) * array->_total_size);
}

void
array_add_at(array_t *array, void *el, size_t i) {
  if(array->_size == array->_total_size) {
    array_realloc(array);
  }
  array->_data[i] = el;
  array->_size++;
}

void
array_append(array_t *array, void *el) {
  array_add_at(array, el, array->_size);
}

void
array_print(array_t *array, array_fn_print fn_print) {
  size_t i;

  for(i = 0; i < array_size(array); i++) {
    fn_print(array_get(array, i));
  }
}
