#include "block_test.h"

#define KEY "key1"
#define VAL 6

void
block_test_foreach(const char *key, uint_array_t *val, void *data) {
  (void) data;
  int i;
  assert_string_equal(key, KEY);

  for(i = 0; i < VAL; i++)
    assert_int_equal(uint_array_get(val, i), i);
}

int
block_test_foreach_rm(const char *key, uint_array_t *val, void *data) {
  block_test_foreach(key, val, data);
  return 1;
}

void
block_test() {
  int i;
  block_t *block;

  block = block_new();

  for(i = 0; i < VAL; i++)
    block_insert(block, KEY, i);

  assert_int_equal(block_size(block), 1);

  block_foreach(block, block_test_foreach, NULL);
  block_foreach_remove(block, block_test_foreach_rm, NULL);

  assert_int_equal(block_size(block), 0);

  block_free(block);
}

int
main(void) {
  const struct CMUnitTest tests[] = {
    cmocka_unit_test(block_test),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}

