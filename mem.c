#include "mem.h"

void
mem_read(mem *result) {
  const char* statm_path = "/proc/self/statm";

  FILE *f = fopen(statm_path,"r");
  if(!f){
    handle_error("proc error");
  }
  if( 7 != fscanf(f,"%lu %lu %lu %lu %lu %lu %lu",
        &result->size,
        &result->resident,
        &result->share,
        &result->text,
        &result->lib,
        &result->data,
        &result->dt)) {
    handle_error("proc error");
  }
  fclose(f);
}

void mem_print() {
  mem my_mem;
  mem_read(&my_mem);
  printf("Memória utilizada: %f\n", my_mem.resident / 1024.0);
}
