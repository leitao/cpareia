#include "block.h"
#include <stdio.h>

block_t *
block_new() {
  block_t *block;

  block = kh_init(32);

  return block;
}

uint_array_t *
block_get(block_t *block, uint32_t key) {
  unsigned int k;

  k = kh_get(32, block, key);

  return k == kh_end(block) ? NULL : kh_value(block, key);
}

void
block_insert(block_t *block, uint32_t key, uint32_t val) {
  int absent, k;

  k = kh_put(32, block, key, &absent);

  if(absent)
    kh_value(block, k) = uint_array_new(1);

  uint_array_push(kh_val(block, k), val);
}

void
block_foreach(block_t *block, block_foreach_fn fn, void *data) {
  uint_array_t *array;
  uint32_t key;

  kh_foreach(block, key, array, fn(key, array, data));
}

void
block_rm(block_t *block, uint32_t key, uint_array_t *array) {
  khint_t k;

  k = kh_get(32, block, key);
  kh_del(32, block, k);
  uint_array_free(array);
}

void
block_foreach_remove(block_t *block, block_foreach_rm_fn fn, void *p) {
  uint_array_t *a;
  uint32_t k;

  kh_foreach(block, k, a, if(fn(k, a, p)) block_rm(block, k, a));
}

int
block_internal_free(uint32_t key, uint_array_t *array, void *data) {
  (void) key;
  (void) array;
  (void) data;

  return 1;
}

void
block_free(block_t *block) {
  block_foreach_remove(block, block_internal_free, NULL);

  kh_destroy(32, block);
}
