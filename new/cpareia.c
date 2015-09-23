#include "cpareia.h"

int
open_file(char *fname, char **buf) {
  int fd;
  struct stat fs;

	fd = open(fname, O_RDONLY);

	if (fd == -1)
		handle_error("open");

  if (fstat(fd, &fs) == -1)
    handle_error("fstat");

	*buf = mmap(NULL, fs.st_size, PROT_READ, MAP_PRIVATE, fd, 0);

  if (*buf == MAP_FAILED)
    handle_error("mmap");

  close(fd);

  return fs.st_size;
}

void new_field (void *parsed, size_t size, void *outfile) {
  char *string;
  (void)(outfile);

  string = (char *)malloc(sizeof(char) * (size + 1));
  strcpy(string, (char *) parsed);

  printf("CAMPO | Tamanho %d:\n%s\n", (int) size, (char *) string);
  free(string);
}

void new_record (int c, void *outfile) {
  (void)(outfile);
  printf("BLAA%c|\n", (char )c);
}

int
main(int argc,  char *argv[]) {
  int size;
  char *buf;
  struct csv_parser p;

  (void)(argc);

  csv_init(&p, CSV_APPEND_NULL);
  csv_set_delim(&p, '\t');

  size = open_file(argv[1], &buf);

  csv_parse(&p, buf, size, new_field, new_record, NULL);

  csv_fini(&p, new_field, new_record, NULL);
  csv_free(&p);


  return 0;
}
