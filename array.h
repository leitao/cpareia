#ifndef _ARRAY_H_
#define _ARRAY_H_
#define INITIAL_SIZE 10000

#include <stdlib.h>

typedef struct array {
  void **_data;
  size_t size, _total_size;
} array;

array * array_new();
array * array_new_prealloc(size_t size);
void array_fini(array *);
void array_free(array *);
void *array_remove_last(array *);
void array_add(array *, void *);
void *array_get(array *, size_t);

#endif
