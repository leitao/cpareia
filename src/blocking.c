#include "blocking.h"

uint32_t
blocking_hash(const char *s) {
  uint32_t h = (uint32_t) *s;
  if(h) for(++s; *s; ++s) h = (h << 5) - h + (uint32_t) *s;
  return h;
}

void
blocking_thread_params_free(blocking_thread_params_t *params) {
  free(params);
}

void
blocking_generate_keys(project_t *project, uint32_t id) {
  size_t i, j, conj;
  conjunction_t *conjunction;
  part_t *part;
  record_t *record;
  char buffer[5], key[1024];

  conj = array_size(project->conjunctions);

  record = array_get(project->d0->records, id);

  record->_keys = malloc(sizeof(uint32_t) * conj);
  bzero(record->_keys, sizeof(uint32_t) * conj);

  for(i = 0; i < conj; i++) {
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
    record->_keys[i] = blocking_hash(key);
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

  for(i = rank; i < size; i += total_ranks - 1) {
    while(!array_get(project->d0->records, i)) sleep(1);
    if(!(i % 1000000)) {
      prop = 100.0 * i / size;
      printf("Registros blocados: %lu/%lu (%2.2f%%)\n", i, size, prop);
    }
    blocking_generate_keys(project, i);
  }

  blocking_thread_params_free(param);

  return NULL;
}

void *
blocking_read_blocks(void *proj) {
  char *k, *p, *key;
  FILE *fh;
  int i, id, total;
  project_t *project;
  char line[200000];

  project = (project_t *) proj;

  fh = fopen(project->args->blocking_file, "r");

  while (fgets(line, sizeof(line), fh))  {
    p = strtok(line, ":");
    key = p;

    p = strtok(NULL, ":");
    total = atoi(p);

    p = strtok(NULL, ":");

    for (i = 0; i < total; i++) {
      k = strtok(p, " ");
      while(k) {
          id = atoi(k + 1);
          block_insert(project->block, key, id);
          k = strtok(NULL, " ");
      }
    }
  }
  return NULL;
}

pthread_t **
blocking_read_file_async(project_t *project) {
  pthread_t **threads;

  threads = malloc(sizeof(pthread_t *) * 1);

  threads[0] = malloc(sizeof(pthread_t));
  pthread_create(threads[0], NULL, blocking_read_blocks, project);

  return threads;
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
