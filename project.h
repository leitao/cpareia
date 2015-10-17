#ifndef _PROJECT_H_
#define _PROJECT_H_

#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>

#include "database.h"
#include "conjunction.h"

typedef struct project {
  unsigned char *name;
  unsigned char *task;
  database *d0;
} project;

project *project_new();
void project_print(project *my_proj);
void project_free(project *my_proj);
void project_parse(project *my_proj, char *file_name);

#endif
