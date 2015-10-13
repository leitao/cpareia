#include "cpareia.h"

int
main(int argc,  char *argv[]) {
  project *my_proj;

  if(argc != 2)
    handle_error("Usage: cpareia XML");

  my_proj = project_new();

  project_fill(my_proj, argv[1]);
  /*project_print(my_proj);*/

  project_free(my_proj);

  return 0;
}
