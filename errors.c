#include "errors.h"

void handle_error(char *msg) {
  printf("%s\n", msg);
  exit(-1);
}
