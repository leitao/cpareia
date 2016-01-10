#include "cpareia.h"

int
delta(struct timeval a, struct timeval b) {
  return (int) (b.tv_sec - a.tv_sec);
}

int
main(int argc, char *argv[]) {
  int i;
  project_t *project;
  pthread_t *read_thread;
  pthread_t **blocking_threads;
  pthread_t **comparator_threads;
  args_t *args;

  args = args_new();

  args_parse(args, argc, argv);

  project = project_new();

  project_parse(project, args->project_file);

  printf("Começando leitura e blocagem\n");
  read_thread = database_read_async(project->d0);

  if(args->blocking_file) {
    blocking_threads = blocking_read_file_async(project);
  } else {
    blocking_threads = blocking_async(project, args->max_threads - 1);
  }

  pthread_join(*read_thread, NULL);

  for(i = 0; i < args->max_threads - 1; i++) {
    pthread_join(*blocking_threads[i], NULL);
    free(blocking_threads[i]);
  }

  free(blocking_threads);
  free(read_thread);
  printf("Blocagem pronta\n\nComeçando comparação e escrita\n");
  comparator_threads = comparator_run_async(project, args->max_threads);

  for(i = 0; i < args->max_threads - 1; i++) {
    pthread_join(*comparator_threads[i], NULL);
    free(comparator_threads[i]);
  }

  free(comparator_threads);

  printf("Comparação pronta\n");

  project_free(project);
  return 0;
}
