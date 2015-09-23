#include "cpareia.h"

int
main(int argc, char *argv[]) {
  char *buf;
  int file_size;

  if (argc != 2) {
    handle_error("argc");
  }


  file_size = open_file(argv[1], &buf);
  parse_file(buf, file_size, 19);

	return 0;
}
