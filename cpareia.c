#include "cpareia.h"
#include <sys/time.h>

double
delta(struct timeval a, struct timeval b) {
  return (b.tv_sec - a.tv_sec);/* + (b.tv_usec - a.tv_usec);*/
}

void callback(void *pool, void *record) {
  pool_push(pool, record);
}

void generate_keys(void *record, void *project) {
  blocking_generate_keys(project, record);
}

int
main(int argc,  char *argv[]) {
  project_t *project;
  struct timeval t0, t1;
  pool_t *pool;

  if(argc != 2)
    handle_error("Usage: cpareia XML");

  project = project_new();

  pool = pool_new(8, project, generate_keys);

  project_parse(project, argv[1]);
  printf("Lendo database e gerando blocagem\n");
  gettimeofday(&t0, NULL);
  database_read(project->d0, callback, pool);
  gettimeofday(&t1, NULL);
  printf("Feito em %f segundos\n",delta(t0, t1));
  mem_print();

  /*project_print(project);
  blocking_print(project);*/

  pool_free(pool);
  project_free(project);

  return 0;
}
