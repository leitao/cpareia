#include "colors.h"

void print_color(const char *color, const char *fmt, ...) {
  va_list ap;

  printf(color);

  va_start(ap, fmt);
  vprintf(fmt, ap);
  va_end(ap);

  printf(ANSI_COLOR_RESET "\n");
}

void printf_red(const char *fmt, ...) {
  va_list ap;
  print_color(ANSI_COLOR_RED, fmt, ap);
}

void printf_green(const char *fmt, ...) {
  va_list ap;
  print_color(ANSI_COLOR_GREEN, fmt, ap);
}

void printf_yellow(const char *fmt, ...) {
  va_list ap;
  print_color(ANSI_COLOR_YELLOW, fmt, ap);
}

void printf_blue(const char *fmt, ...) {
  va_list ap;
  print_color(ANSI_COLOR_BLUE, fmt, ap);
}

void printf_magenta(const char *fmt, ...) {
  va_list ap;
  print_color(ANSI_COLOR_MAGENTA, fmt, ap);
}

void printf_cyan(const char *fmt, ...) {
  va_list ap;
  print_color(ANSI_COLOR_CYAN, fmt, ap);
}
