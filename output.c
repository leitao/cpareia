#include "output.h"

FILE *
output_get_file(output_t *output, int file) {
  return output->files[file];
}

void
output_write(result_t *result, FILE *file) {
  int i;

  fprintf(
      file,
      "%c %c %s %s %f",
      result->status,
      'X', result->id1,
      result->id2,
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

  return output;
}

void
output_open_files(output_t *output, int num_files) {
  int i;
  size_t size;
  char *name;

  size = strlen(output->filename);

  name = malloc(sizeof(char) * (size + 10));

  output->num_files = num_files;
  output->files = malloc(sizeof(FILE *) * num_files);

  for(i = 0; i < num_files; i++) {
    sprintf(name, "%s%d", output->filename, i);

    if(!(output->files[i] = fopen(name, "w"))) {
      handle_error("Erro ao abrir arquivo %s\n", name);
    }
  }

  free(name);
}

void
output_print(output_t *output) {
  printf("  Filename: %s\n", output->filename);
  printf("  Num files: %d\n", output->num_files);
  printf("  Min: %f\n", output->min);
  printf("  Max: %f\n", output->max);
}

void
output_free(output_t *output) {
  int i;

  for(i = 0; i < output->num_files; i++) {
    fclose(output->files[i]);
  }
  free(output->files);

  free(output->filename);
  free(output);
}
