#include "output.h"

void
output_write(void *res, void *f) {
  int i;
  result_t *result;
  FILE *file;

  result = (result_t *) res;
  file = (FILE *) f;

  fprintf(
      file,
      "%c %c %s %s %f",
      result->status,
      'X',
      record_get_field(result->r1, 0),
      record_get_field(result->r2, 0),
      result->score);

  for(i = 0; i < result->num_scores; i++) {
    fprintf(file, " %f", result->scores[i]);
  }
  fprintf(file, "\n");

  result_free(result);
}

output_t *
output_new(char *filename, double min, double max) {
  output_t *output;

  output = malloc(sizeof(output_t));

  output->filename = filename;
  output->min = min;
  output->max = max;

  if(!(output->file = fopen(output->filename, "w"))) {
    handle_error("Erro ao abrir arquivo %s\n", output->filename);
  }

  output->pool = pool_new(1, output->file, output_write);

  return output;
}

void
output_push(output_t *output, result_t *result) {
  pool_push(output->pool, result);
}

void
output_print(output_t *output) {
  printf("  Filename: %s\n", output->filename);
  printf("  Min: %f\n", output->min);
  printf("  Max: %f\n", output->max);
}

void
output_free(output_t *output) {
  pool_free(output->pool);
  fclose(output->file);

  free(output->filename);
  free(output);
}
