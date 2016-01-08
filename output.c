#include "output.h"

gzFile
output_get_file(output_t *output, int file) {
  return output->files[file];
}

void
output_write(result_t *result, gzFile file) {
  int i;

  gzprintf(
      file,
      "%c %c %s %s %f",
      result->status,
      'X', result->id1,
      result->id2,
      result->score);

  for(i = 0; i < result->num_scores; i++) {
    gzprintf(file, " %f", result->scores[i]);
  }
  gzprintf(file, "\n");

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
  output->files = malloc(sizeof(gzFile) * num_files);

  for(i = 0; i < num_files; i++) {
    sprintf(name, "%s%d.gz", output->filename, i);

    if(!(output->files[i] = gzopen(name, "wb"))) {
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
    gzclose(output->files[i]);
  }
  free(output->files);

  free(output->filename);
  free(output);
}
