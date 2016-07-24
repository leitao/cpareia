#include "record_test.h"

void record_test() {
  record_t *record;
  int i, size, field_size;
  char *fields[] = {"ab", "abc", "abcd", "abcde", "ab", "c"};

  size = sizeof(fields) / sizeof(*fields);

  record = record_new(size);

  assert_int_equal(record->num_fields, size);

  for(i = 0; i < size; i++)
    record_add_field(record, fields[i]);

  for(i = 0; i < size; i++)
    assert_int_equal(strcmp(record_get_field(record, i), fields[i]), 0);

  for(i = 0; i < size; i++) {
    field_size = record_get_field_size(record, i);
    assert_int_equal(field_size, strlen(fields[i]));
  }

  record_free(record);
}

int main(void) {
  const struct CMUnitTest tests[] = {
    cmocka_unit_test(record_test),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
