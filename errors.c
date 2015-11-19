#include "errors.h"

void handle_error(char *msg) {
  printf_red(msg);
  exit(-1);
}
