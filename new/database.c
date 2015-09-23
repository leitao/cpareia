#include "database.h"

database *database_new() {
  database *my_db;

  my_db = (database *) malloc(sizeof(database));

  my_db->size = 0;
  my_db->records = NULL;

  return my_db;
}

void database_add_record(database *my_db, record *rec) {

}
