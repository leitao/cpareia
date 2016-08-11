#ifndef _UINT_ARRAY_H_
#define _UINT_ARRAY_H_

#include <stdlib.h>
#include <strings.h>
#include <stdint.h>

#include "kvec.h"

typedef kvec_t(uint32_t) uint_array_t;

uint_array_t *uint_array_new(size_t);
void uint_array_free(uint_array_t *);
void uint_array_push(uint_array_t *, uint32_t);

#define uint_array_size(array) (kv_size(*(array)))
#define uint_array_get(array, i) (kv_A(*(array), (i)))

#endif
