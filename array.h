#ifndef _ARRAY_H_
#define _ARRAY_H_
#define INITIAL_SIZE 10000

#include <stdlib.h>

typedef struct array_t {
  void **_data;
  size_t _size, _total_size;
} array_t;

typedef void (*array_fn_print)(void *);

array_t * array_new();
array_t * array_new_prealloc(size_t);
void array_print(array_t *, array_fn_print);
void array_fini(array_t *);
void array_free(array_t *);
void array_append(array_t *, void *);
void array_add_at(array_t *, void *, size_t);
void *array_get(array_t *, size_t);
size_t array_size(array_t *);

#endif
