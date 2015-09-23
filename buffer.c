#include "buffer.h"

char *
buffer_to_string(buffer *buf) {
  char *string;
  int i, size;

  /*Empty string*/
  size = !(buf->begin && buf->end) ? 1 : buf->end - buf->begin + 2;

  string = (char *) malloc(sizeof(char) * size);

  for (i = 0; i < size - 1; i++) {
    string[i] = buf->begin[i];
  }
  string[size - 1] = 0;

  return string;
}

void
buffer_print(buffer *buf) {
  char *string;

  string = buffer_to_string(buf);
  printf("%s\n", string);
  free(string);
}
