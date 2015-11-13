#include "cpareia.h"
#include <sys/time.h>

double
delta(struct timeval a, struct timeval b) {
  return (b.tv_sec - a.tv_sec);/* + (b.tv_usec - a.tv_usec);*/
}

int
main(int argc,  char *argv[]) {
  project_t *project;
  struct timeval t0, t1;

  if(argc != 2)
    handle_error("Usage: cpareia XML");

  project = project_new();

  project_parse(project, argv[1]);
  printf("Lendo database\n");
  gettimeofday(&t0, NULL);
  database_read(project->d0);
  gettimeofday(&t1, NULL);
  printf("Database lida em %f segundos\n",delta(t0, t1));
  mem_print();

  /*project_print(project);*/

  printf("Gerando chaves de blocagens\n");
  gettimeofday(&t0, NULL);
  blocking_generate(project);
  gettimeofday(&t1, NULL);
  printf("Chaves geradas em %f segundos\n", delta(t0, t1));

  /*blocking_print(project);*/
  mem_print();

  project_free(project);

  return 0;
}
