#ifndef _BLOCK_H_
#define _BLOCK_H_

#include <stdint.h>
#include "khash.h"
#include "uint_array.h"

KHASH_MAP_INIT_INT(32, uint_array_t *)

typedef void (*block_foreach_fn)(uint32_t key, uint_array_t *array, void *p);
typedef int (*block_foreach_rm_fn)(uint32_t key, uint_array_t *array, void *p);

typedef khash_t(32) block_t;

block_t *block_new();
void block_insert(block_t *block, uint32_t key, uint32_t value);
size_t block_size(block_t *block);
uint_array_t *block_get(block_t *block, uint32_t key);

#define block_size(block) kh_size((block))

void block_foreach(block_t *block, block_foreach_fn, void *p);
void block_free(block_t *block);
void block_foreach_remove(block_t *block, block_foreach_rm_fn, void *p);


#endif
