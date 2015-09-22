#include "cpareia.h"

int
main(int argc, char *argv[]) {

  if (argc != 2) {
    handle_error("argc");
  }

  open_file(argv[1], print_pointers);

	return 0;
}
