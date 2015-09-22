#include "cpareia.h"

int
main(int argc, char *argv[]) {

  if (argc != 2) {
    handle_error("argc");
  }

  _current_record = NULL;

  open_file(argv[1], parse_line);

	return 0;
}
