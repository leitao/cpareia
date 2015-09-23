#ifndef _FILE_H_
#define _FILE_H_

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

#include "string.h"
#include "record.h"
#include "buffer.h"

#define handle_error(msg) \
  do { perror(msg); exit(EXIT_FAILURE); } while (0)

void parse_line(buffer *, void *);
int open_file(char *, char **);
record **parse_file(char *, int, int);
void close_file(char *, int);

#endif
