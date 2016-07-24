#include "database_test.h"

void
database_test() {
  database_t *database;
  char *filename;

  filename = strdup("fixtures/database.csv");

  database = database_new(filename, 4, 3, ',');

  database_read(database);

  database_free(database);
}

int
main(void) {
  const struct CMUnitTest tests[] = {
    cmocka_unit_test(database_test),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
