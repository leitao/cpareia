#include "cpareia.h"
#include <sys/time.h>

double
delta(struct timeval a, struct timeval b) {
  return (b.tv_sec - a.tv_sec);/* + (b.tv_usec - a.tv_usec);*/
}

int
main(int argc,  char *argv[]) {
  project *my_proj;
  struct timeval t0, t1;

  if(argc != 2)
    handle_error("Usage: cpareia XML");

  my_proj = project_new();

  printf("Parseando projeto\n");
  gettimeofday(&t0, NULL);
  project_parse(my_proj, argv[1]);
  gettimeofday(&t1, NULL);
  printf("Parser completo em %f segundos\n", delta(t0, t1));

  printf("Lendo database\n");
  gettimeofday(&t0, NULL);
  database_read(my_proj->d0);
  gettimeofday(&t1, NULL);
  printf("Database lida em %f segundos\n",delta(t0, t1));

  /*project_print(my_proj);*/

  printf("Gerando chaves de blocagens\n");
  gettimeofday(&t0, NULL);
  blocking_generate(my_proj);
  gettimeofday(&t1, NULL);
  printf("Chaves geradas em %f segundos\n", delta(t0, t1));

  project_free(my_proj);

  return 0;
}
