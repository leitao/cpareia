#include "file.h"

char *
each_token(char *begin, char *end, char *delim,
    void (*call_back)(char*, char*)) {

  char *current;

  current = begin;

  while (1) {
    if (*current != *delim) {
      current++;
      if (current < end) continue;
    }

    call_back(begin, current - 1);

    current++;

    return (current >= end) ? NULL: current;
  }
}

char *
pointers_to_string(char *begin, char *end) {
  char *string;
  int i, size;

  size = end - begin + 2;

  assert(end > begin);

  string = (char *) malloc(sizeof(char) * size);

  for (i = 0; i < size - 1; i++) {
    string[i] = begin[i];
  }
  string[size - 1] = 0;

  return string;
}

void
print_pointers(char *begin, char *end) {
  char *string;

  string = pointers_to_string(begin, end);
  printf("%s\n", string);
  free(string);
}

void
open_file(char *fname, void (*call_back)(char*, char*)) {
  char *buf, *buf_end, *begin, delim;

  int fd;
  struct stat fs;

	fd = open(fname, O_RDONLY);

	if (fd == -1) {
		handle_error("open");
  }

  if (fstat(fd, &fs) == -1) {
    handle_error("fstat");
  }

	buf = mmap(NULL, fs.st_size, PROT_READ, MAP_PRIVATE, fd, 0);

  if (buf == MAP_FAILED) {
    handle_error("mmap");
  }

  begin = buf;
  buf_end = buf + fs.st_size;
  delim = '\n';

  while ((begin = each_token(begin, buf_end, &delim, call_back)));

  munmap(buf, fs.st_size);
  close(fd);
}
