#include "blocking.h"

void
blocking_thread_params_free(blocking_thread_params_t *params) {
  free(params);
}

void
blocking_generate_keys(project_t *project, uint32_t id) {
  size_t i, j;
  conjunction_t *conjunction;
  part_t *part;
  record_t *record;
  char buffer[5], key[1024];

  record = array_get(project->d0->records, id);

  for(i = 0; i < array_size(project->conjunctions); i++) {
    conjunction = array_get(project->conjunctions, i);
    key[0] = '\0';

    for(j = 0; j < array_size(conjunction->parts); j++) {
      part = array_get(conjunction->parts, j);

      if(!part->transform) {
        strcat(key, record_get_field(record, part->field));
      } else if(!strcmp(part->transform, "brsoundex")) {
        brsoundex(record_get_field(record, part->field), buffer, 5);
        strcat(key, buffer);
      }
      else {
        handle_error("Unknown transformation");
      }
    }
    if(strlen(key)) {
      block_insert(project->block, key, id);
    }
  }
}

void *
blocking_generate_all_keys(void *data) {
  size_t i, size;
  int rank, total_ranks;
  double prop;
  project_t *project;
  blocking_thread_params_t *param;

  param = data;

  rank = param->rank;
  project = param->project;
  total_ranks = param->total_ranks;
  size = project->d0->nrows;

  for(i = rank; i < size; i += total_ranks) {
    while(!array_get(project->d0->records, i)) usleep(0.1);
    if(!(i % 1000000)) {
      prop = 100.0 * i / size;
      printf("Registros blocados: %lu/%lu (%2.2f%%)\n", i, size, prop);
    }
    blocking_generate_keys(project, i);
  }

  printf("Registros blocados: %lu/%lu (%2.2f%%)\n", size, size, 100.0);

  blocking_thread_params_free(param);

  return NULL;
}

pthread_t **
blocking_async(project_t *project) {
  pthread_t **threads;
  blocking_thread_params_t *param;
  int i, total_rank;

  total_rank = project->args->max_threads - 1;

  threads = malloc(sizeof(pthread_t *) * total_rank);

  for(i = 0; i < total_rank; i++) {
    threads[i] = malloc(sizeof(pthread_t));
    param = malloc(sizeof(blocking_thread_params_t));
    param->project = project;
    param->rank = i;
    param->total_ranks = total_rank;

    pthread_create(threads[i], NULL, blocking_generate_all_keys, param);
  }

  return threads;
}
