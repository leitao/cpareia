#include "cpareia.h"
#include <sys/time.h>

int
delta(struct timeval a, struct timeval b) {
  return (int) (b.tv_sec - a.tv_sec);
}

int
main(int argc,  char *argv[]) {
  int i, num_blocking_threads;
  project_t *project;
  struct timeval t0, t1;
  pthread_t *read_thread;
  pthread_t **blocking_threads;
  void *data;

  if(argc != 2)
    handle_error("Usage: cpareia XML");

  num_blocking_threads = 8;

  project = project_new();

  project_parse(project, argv[1]);

  read_thread = database_read_async(project->d0);

  blocking_threads = blocking_generate_keys_async(
      project,
      num_blocking_threads);

  pthread_join(*read_thread, NULL);

  for(i = 0; i < num_blocking_threads; i++) {
    pthread_join(*blocking_threads[i], &data);
    free(blocking_threads[i]);
    free(data);
  }

  free(blocking_threads);
  free(read_thread);

  printf_green("Começando comparação\n");
  gettimeofday(&t0, NULL);
  comparator_start(project);
  gettimeofday(&t1, NULL);
  printf_green("Comparação finalizada em %f segundos\n", delta(t0, t1));
  mem_print();

  /*
  project_free(project);
  */
  return 0;
}
