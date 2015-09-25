#include "cpareia.h"

int
main(int argc,  char *argv[]) {
  database *db;

  (void)(argc);

  db = read_database(argv[1]);


  database_free(db);

  return 0;
}
