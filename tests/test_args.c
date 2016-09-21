#include "test_args.h"

void
test_args() {
  char *argv[] = {"prog_name", "-t", "42", "-p", "fixtures/foo.xml", NULL};
  int argc = 5;
  args_t *args;

  args = args_new();

  assert_int_equal(args->max_threads, sysconf(_SC_NPROCESSORS_ONLN));

  args_parse(args, argc, argv);

  assert_int_equal(args->max_threads, 42);
  assert_string_equal(args->project_file, "fixtures/foo.xml");

  args_free(args);
}

int main(void) {
  const struct CMUnitTest tests[] = {
    cmocka_unit_test(test_args),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
