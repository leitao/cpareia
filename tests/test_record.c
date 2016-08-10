#include "test_record.h"

void
record_test_new_full() {
  record_t *record;
  char fields[] = "ab\0abc\0abcd\0abcde\0ab\0c";
  uint8_t *sizes;
  size_t num_fields = 6;

  sizes = malloc(sizeof(uint8_t) * 6);

  sizes[0] = 3;
  sizes[1] = 4;
  sizes[2] = 5;
  sizes[3] = 6;
  sizes[4] = 3;
  sizes[5] = 2;

  record = record_new(num_fields, fields, sizes);

  assert_string_equal("ab", record_get_field(record, 0));
  assert_string_equal("abc", record_get_field(record, 1));
  assert_string_equal("abcd", record_get_field(record, 2));
  assert_string_equal("abcde", record_get_field(record, 3));
  assert_string_equal("ab", record_get_field(record, 4));
  assert_string_equal("c", record_get_field(record, 5));

  record_free(record);
}

int main(void) {
  const struct CMUnitTest tests[] = {
    cmocka_unit_test(record_test_new_full),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
