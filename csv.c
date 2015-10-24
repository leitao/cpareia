#include "csv.h"

size_t
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

csv_fields *csv_fields_new(size_t num_fields) {
  csv_fields *my_fields;

  my_fields = (csv_fields *) malloc(sizeof(csv_fields));

  my_fields->fields = (char **) malloc(sizeof(char *) * num_fields);

  my_fields->size = num_fields;

  return my_fields;
}

csv_row *
csv_row_new(char *begin, char *end) {
  csv_row *my_row;

  my_row = (csv_row *) malloc(sizeof(csv_row));
  my_row->begin = begin;
  my_row->end = end;

  return my_row;
}

void
csv_row_free(csv_row *row) {
  free(row);
}

void
csv_free(csv *my_csv) {
  munmap(my_csv->buf, my_csv->size);
  free(my_csv);
}

void
csv_fields_free(csv_fields *fields) {
  size_t i;

  for(i = 0; i < fields->size; i++) {
    free(fields->fields[i]);
  }
  free(fields->fields);
  free(fields);
}

csv *
csv_new(char *filename, char sep, size_t num_fields) {
  csv *my_csv;

  my_csv = (csv *) malloc(sizeof(csv));

  my_csv->size = open_file(filename, &(my_csv->buf));
  my_csv->current = my_csv->buf;
  my_csv->end = my_csv->buf + my_csv->size - 1;
  my_csv->sep = sep;
  my_csv->num_fields = num_fields;

  return my_csv;
}

void
csv_row_print(csv_row *my_row) {
  char *current;

  current = my_row->begin;

  while(current <= my_row->end) {
    printf("%c", *current);
    current++;
  }
  printf("\n");
}

csv_row *csv_get_row(csv *my_csv) {
  csv_row *my_row;
  char *begin, *end;

  if(my_csv->current == my_csv->end)
    return NULL;

  begin = end = my_csv->current;

  while(*end != '\n')
    end++;

  my_row = csv_row_new(begin, end - 1);

  /* Skipping multiple \n's */
  while(*end == '\n' && end != my_csv->end)
    end++;

  my_csv->current = end;

  return my_row;
}

csv_fields *csv_row_get_fields(
    csv_row *row,
    char sep,
    size_t num_fields) {

  csv_fields *my_fields;
  char *begin, *current;
  size_t i, size;

  i = 0;
  my_fields = csv_fields_new(num_fields);
  begin = current = row->begin;

  while(current <= row->end) {
    if(*current == sep || current == row->end) {
      if(begin == current) {
        my_fields->fields[i++] = strdup("\0");
      }
      else {
        size = current - begin;
        my_fields->fields[i++] = strndup(begin, size);
      }
      if(*current == sep && current == row->end) {
        my_fields->fields[i++] = strdup("\0");
      }
      begin = current + 1;
    }
    current++;
  }

  return my_fields;
}

int
main(int argv, char *argc[]) {
  csv *my_csv;
  csv_row *my_row;
  csv_fields *my_fields;
  size_t i;

  if(argv != 2)
    handle_error("argv");

  my_csv = csv_new(argc[1], ':', 7);

  while((my_row = csv_get_row(my_csv)) != NULL) {
    csv_row_print(my_row);
    my_fields = csv_row_get_fields(my_row, ':', 7);

    for(i = 0; i < 7; i++) {
      puts(my_fields->fields[i]);
    }
    csv_fields_free(my_fields);
    csv_row_free(my_row);
  }
  csv_free(my_csv);

  return 0;
}
