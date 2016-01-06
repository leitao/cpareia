#include "cpareia.h"

int
delta(struct timeval a, struct timeval b) {
  return (int) (b.tv_sec - a.tv_sec);
}

int
main(int argc,  char *argv[]) {
  int i;
  project_t *project;
  pthread_t *read_thread;
  pthread_t **blocking_threads;
  long max_threads;

  if(argc != 2)
    handle_error("Usage: cpareia XML\n");

  max_threads = sysconf(_SC_NPROCESSORS_ONLN);

  project = project_new();

  project_parse(project, argv[1]);

  read_thread = database_read_async(project->d0);

  blocking_threads = blocking_async(project, max_threads - 1);

  pthread_join(*read_thread, NULL);
  printf("leitura pronta\n");

  for(i = 0; i < max_threads - 1; i++) {
    pthread_join(*blocking_threads[i], NULL);
    free(blocking_threads[i]);
  }

  free(blocking_threads);
  free(read_thread);
  printf("blocagem pronta\n");

  comparator_run(project, max_threads);

  project_free(project);
  return 0;
}
