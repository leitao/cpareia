#ifndef _ARRAY_H_
#define _ARRAY_H_

#include <stdlib.h>
#include <strings.h>

#define array_get(array, i) array->_data[i]
#define array_size(array) array->_size
#define array_total_size(array) array->_total_size

typedef struct array_t {
  void **_data;
  size_t _size, _total_size;
} array_t;

typedef void (*array_fn_print)(void *);

array_t *array_new(size_t);
array_t *array_new_zeroed(size_t);
void array_print(array_t *, array_fn_print);
void array_free(array_t *);
void array_append(array_t *, void *);
void array_add_at(array_t *, void *, size_t);

#endif
