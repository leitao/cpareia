#ifndef _PROJECT_H_
#define _PROJECT_H_

typedef struct project {
  unsigned char *name;
  unsigned char *task;
} project;

#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>

project *project_new();
void project_print(project *my_proj);
void project_free(project *my_proj);
void project_fill(project *my_proj, char *file_name);

#include "cpareia.h"
#endif
