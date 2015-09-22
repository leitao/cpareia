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

#include "record.h"

record **_records;
record *_current_record;

#define handle_error(msg) \
  do { perror(msg); exit(EXIT_FAILURE); } while (0)

char *each_token(char *, char *, char *, void (*)(char*, char*));
char *pointers_to_string(char *, char *);
void print_pointers(char *, char *);
void parse_line(char *, char *);
void open_file(char *, void (*)(char*, char*));

#endif
