#ifndef _ARRAY_H_
#define _ARRAY_H_

#include <stdlib.h>
#include <strings.h>

#include "kvec.h"

kvec_t(void *) array;

typedef struct array_t {
	kvec_t(void *) _data;
} array_t;

array_t *array_new(size_t);
array_t *array_new_zeroed(size_t);
void array_free(array_t *);
void array_push(array_t *, void *);

#define array_size(array) (kv_size(array->_data))
#define array_get(array, i) (kv_A(array->_data, i))

#endif
