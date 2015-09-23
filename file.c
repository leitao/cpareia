#include "file.h"

void update_current_record(buffer *buf, void *data) {
  char *string;
  record *rec;

  rec = (record *) data;

  string = buffer_to_string(buf);
  record_add_field(rec, string);
}

void parse_line(buffer *buf, void *data) {
  char *current;
  record *rec;

  (void)(data);

  current = buf->begin;

  rec = record_new(0, 19);

  while ((current = each_token(current, buf->end, '\t', (void *) rec, update_current_record)));

  /*
  record_print(rec);
  */
  record_free(rec);
}

record **parse_file(char *buf, int size, int num_fields) {
  char *begin, *buf_end;
  record **db;

  begin = buf;
  buf_end = buf + size;

  while ((begin = each_token(begin, buf_end, '\n', (void *) &num_fields,
          parse_line)));

  return db;
}

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

void close_file(char *buf, int size) {
  munmap(buf, size);
}
