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
