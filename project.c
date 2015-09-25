#include "project.h"

project *project_new() {
  project *my_proj;

  my_proj = (project *) malloc(sizeof(project));

  return my_proj;
}

void project_print(project *my_proj) {
  printf("Project %s\n", my_proj->name);
  printf("Task: %s\n", my_proj->task);
}

void project_free(project *my_proj) {
  free(my_proj->task);
  free(my_proj->name);
  free(my_proj);
}

void project_fill(project *my_proj, char *file_name) {
  xmlDocPtr doc;
  xmlXPathContextPtr xpath_ctx;
  xmlXPathObjectPtr xpath;

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

  xmlXPathFreeObject(xpath);
  xmlXPathFreeContext(xpath_ctx);
  xmlFreeDoc(doc);
  xmlCleanupParser();
}
