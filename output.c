#include "output.h"

output_t *
output_new(char *filename, double min, double max) {
  output_t *output;

  output = malloc(sizeof(output_t));

  output->filename = filename;
  output->min = min;
  output->max = max;

  return output;
}

void
output_print(output_t *output) {
  printf("  Filename: %s\n", output->filename);
  printf("  Min: %f\n", output->min);
  printf("  Max: %f\n", output->max);
}

void
output_free(output_t *output) {
  free(output->filename);
  free(output);
}
