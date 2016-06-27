#include "array.h"

array_t *
array_new(size_t size) {
  array_t *array;

  array = malloc(sizeof(array_t));

  kv_init(array->_data);

  kv_resize(void *, array->_data, size);

  return array;
}

array_t *
array_new_zeroed(size_t size) {
  array_t *array;

  array = array_new(size);
  bzero(array->_data.a, sizeof(void *) * size);

  return array;
}

void
array_push(array_t *array, void *el) {
  kv_push(void *, array->_data, el);
}

void
array_free(array_t *array) {
  kv_destroy(array->_data);
  free(array);
}
