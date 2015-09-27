#include "project.h"

project *project_new() {
  project *my_proj;

  my_proj = (project *) malloc(sizeof(project));

  return my_proj;
}

void project_print(project *my_proj) {
  printf("Project: %s\n", my_proj->name);
  printf("Task: %s\n", my_proj->task);
  printf("D0:\n");
  database_print(my_proj->d0);
}

void project_free(project *my_proj) {
  free(my_proj->task);
  free(my_proj->name);
  database_free(my_proj->d0);
  free(my_proj);
}

void project_fill(project *my_proj, char *file_name) {
  xmlDocPtr doc;
  xmlXPathContextPtr xpath_ctx;
  xmlXPathObjectPtr xpath;
  int i;
  xmlChar *filename, *sep;

  xmlInitParser();

  if((doc = xmlParseFile(file_name)) == NULL)
    handle_error("Unable to parse XML");

  if((xpath_ctx = xmlXPathNewContext(doc)) == NULL)
    handle_error("Unable to create XPath");

  xpath = xmlXPathEvalExpression(BAD_CAST "/project", xpath_ctx);

  my_proj->name = xmlGetProp(
      xpath->nodesetval->nodeTab[0],
      BAD_CAST "name");

  my_proj->task = xmlGetProp(
      xpath->nodesetval->nodeTab[0],
      BAD_CAST "task");

  if(strcmp((const char *)my_proj->task, "deduplication")) {
    handle_error("We only support 'deduplication' right now");
  }

  xmlXPathFreeObject(xpath);

  xpath = xmlXPathEvalExpression(
      BAD_CAST "/project/data-sources/data-source[@id=0]",
      xpath_ctx);

  filename = xmlGetProp(
      xpath->nodesetval->nodeTab[0],
      BAD_CAST "file");

  sep = xmlGetProp(
      xpath->nodesetval->nodeTab[0],
      BAD_CAST "field-separator");

  xmlXPathFreeObject(xpath);

  xpath = xmlXPathEvalExpression(
      BAD_CAST "/project/data-sources/data-source[@id=0]/fields/field",
      xpath_ctx);

  my_proj->d0 = database_new(xpath->nodesetval->nodeNr);

  my_proj->d0->filename = filename;
  my_proj->d0->sep = *sep;

  for(i = 0; i < xpath->nodesetval->nodeNr; i++) {
    my_proj->d0->fields[i] = xmlGetProp(
        xpath->nodesetval->nodeTab[i],
        BAD_CAST "name");
  }

  database_read(my_proj->d0);

  free(sep);
  xmlXPathFreeObject(xpath);
  xmlXPathFreeContext(xpath_ctx);
  xmlFreeDoc(doc);
  xmlCleanupParser();
}
