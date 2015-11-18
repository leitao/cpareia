#include "colors.h"

void printf_red(const char *msg) {
  printf("%s%s%s\n",ANSI_COLOR_RED, msg, ANSI_COLOR_RESET "\n");
}

void printf_green(const char *msg) {
  printf("%s%s%s\n",ANSI_COLOR_GREEN, msg, ANSI_COLOR_RESET "\n");
}

void printf_yellow(const char *msg) {
  printf("%s%s%s\n",ANSI_COLOR_YELLOW, msg, ANSI_COLOR_RESET "\n");
}

void printf_blue(const char *msg) {
  printf("%s%s%s\n",ANSI_COLOR_BLUE, msg, ANSI_COLOR_RESET "\n");
}

void printf_magenta(const char *msg) {
  printf("%s%s%s\n",ANSI_COLOR_MAGENTA, msg, ANSI_COLOR_RESET "\n");
}

void printf_cyan(const char *msg) {
  printf("%s%s%s\n",ANSI_COLOR_CYAN, msg, ANSI_COLOR_RESET "\n");
}
