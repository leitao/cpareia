#ifndef _ARRAY_H_
#define _ARRAY_H_
#define INITIAL_SIZE 10000

#include <stdlib.h>

typedef struct array {
  void **data;
  size_t size, _total_size;
} array;

array * array_new();
void array_fini(array *);
void array_free(array *);
void *array_remove_last(array *);
void array_add(array *, void *);

#endif
