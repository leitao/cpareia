#include "string.h"

char *
each_token(char *begin, char *end, char delim, void *data,
    void (*call_back)(char*, char*, void *)) {

  char *current;

  current = begin;

  while (1) {
    if (*current != delim) {
      current++;
      if (current <= end) continue;
    }

    call_back(begin, current - 1, data);

    current++;

    /* two delimiters together */
    while(*current == delim && current <= end) {
      call_back(NULL, NULL, data);
      current++;
    }

    return (current >= end) ? NULL: current;
  }
}

char *
pointers_to_string(char *begin, char *end) {
  char *string;
  int i, size;

  /*Empty string*/
  size = !(begin && end) ? 1 : end - begin + 2;

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
