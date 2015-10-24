#include "csv.h"

csv_fields *csv_fields_new() {
  csv_fields *my_fields;

  my_fields = (csv_fields *) malloc(sizeof(csv_fields));

  my_fields->size = 0;

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

csv *
csv_new(char *filename, char sep) {
  csv *my_csv;

  my_csv = (csv *) malloc(sizeof(csv));

  my_csv->size = open_file(filename, &(my_csv->buf));
  my_csv->current = my_csv->buf;
  my_csv->end = my_csv->buf + my_csv->size - 1;
  my_csv->sep = sep;

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

csv_fields *csv_row_get_fields(csv_row *row, char sep) {
  csv_fields *my_fields;
  char *begin, *current;

  my_fields = csv_fields_new();

  begin = current = row->begin;

  while(current <= row->end) {
    if(*current == sep || current == row->end) {
      if(begin == current) {
        printf("encontrei vazio\n");
      }
      else {
        printf("encontrei. de %c até %c\n", *begin, *(current - 1));
      }
      if(*current == sep && current == row->end) {
        printf("Final vazio\n");
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

  if(argv != 2)
    handle_error("argv");

  my_csv = csv_new(argc[1], ':');

  while((my_row = csv_get_row(my_csv)) != NULL) {
    csv_row_print(my_row);
    my_fields = csv_row_get_fields(my_row, ':');
    free(my_row);
  }

  return 0;
}
