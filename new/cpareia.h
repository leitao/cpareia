#ifndef _CPAREIA_H_
#define _CPAREIA_H_

#include <sys/mman.h>
#include <sys/stat.h>
#include <csv.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define handle_error(msg) \
  do { perror(msg); exit(EXIT_FAILURE); } while (0)

#endif
