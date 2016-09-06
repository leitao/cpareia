#include "test_database.h"

void
test_database() {
  database_t *database;
  char *filename;
  record_t *record;

  filename = strdup("fixtures/database.csv");

  database = database_new(filename, 4, 3, ',');

  database_read(database);

  assert_int_equal(array_size(database->records), 3);

  record = array_get(database->records, 0);
  assert_string_equal(record_get_field(record, 0), "field1");
  assert_string_equal(record_get_field(record, 1), "field2");
  assert_string_equal(record_get_field(record, 2), "field3");
  assert_string_equal(record_get_field(record, 3), "field4");

  record = array_get(database->records, 1);
  assert_string_equal(record_get_field(record, 0), "foo1");
  assert_string_equal(record_get_field(record, 1), "foo2");
  assert_string_equal(record_get_field(record, 2), "foo3");
  assert_string_equal(record_get_field(record, 3), "foo4");

  record = array_get(database->records, 2);
  assert_string_equal(record_get_field(record, 0), "bar1");
  assert_string_equal(record_get_field(record, 1), "bar2");
  assert_string_equal(record_get_field(record, 2), "bar3");
  assert_string_equal(record_get_field(record, 3), "bar4");

  database_free(database);
}

int
main(void) {
  const struct CMUnitTest tests[] = {
    cmocka_unit_test(test_database),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
