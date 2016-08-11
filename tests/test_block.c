#include "test_block.h"

void
block_test_foreach(uint32_t key, uint_array_t *val, void *data) {
  (void) data;

  assert_int_equal(uint_array_get(val, 0), key);
}

int
block_test_foreach_rm(uint32_t key, uint_array_t *val, void *data) {
  block_test_foreach(key, val, data);

  return 1;
}

void
test_block() {
  int i;
  block_t *block;

  block = block_new();

  for(i = 0; i < 100; i++)
    block_insert(block, i, i);

  assert_int_equal(block_size(block), 100);

  block_foreach(block, block_test_foreach, NULL);

  block_foreach_remove(block, block_test_foreach_rm, NULL);

  assert_int_equal(block_size(block), 0);

  block_free(block);
}

int
main(void) {
  const struct CMUnitTest tests[] = {
    cmocka_unit_test(test_block),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}

