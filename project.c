#include "project.h"

project *
project_new() {
  project *my_proj;

  my_proj = (project *) malloc(sizeof(project));

  my_proj->conjunctions = array_new();

  return my_proj;
}

void
project_print(project *my_proj) {
  size_t i;

  printf("Project: %s\n", my_proj->name);
  printf("Task: %s\n", my_proj->task);
  for(i = 0; i < my_proj->conjunctions->size; i++) {
    printf("Conjunction %d:\n", (int) i);
    conjunction_print(array_get(my_proj->conjunctions, i));
  }

  printf("D0:\n");
  database_print(my_proj->d0);
}

void
project_free(project *my_proj) {
  size_t i;

  free(my_proj->task);
  free(my_proj->name);
  database_free(my_proj->d0);

  for(i = 0; i < my_proj->conjunctions->size; i++) {
    conjunction_free(array_get(my_proj->conjunctions, i));
  }

  array_free(my_proj->conjunctions);
  free(my_proj);
}

void
project_add_conjunction(project *my_proj, conjunction *conj) {
  array_add(my_proj->conjunctions, conj);
}

void
project_parse(project *my_proj, char *file_name) {
  xmlDocPtr doc;
  xmlXPathContextPtr xpath_ctx;
  xmlXPathObjectPtr xpath;
  xmlNode *part_node;
  xmlChar *filename, *sep;
  char *field_name, *transform, *size;
  conjunction *conj;
  int i;

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

  if(strcmp((char *)my_proj->task, "deduplication")) {
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

  if(!strcmp((char *) sep, "\\t")) {
    my_proj->d0->sep = '\t';
  } else {
    my_proj->d0->sep = *sep;
  }

  for(i = 0; i < xpath->nodesetval->nodeNr; i++) {
    my_proj->d0->fields[i] = xmlGetProp(
        xpath->nodesetval->nodeTab[i],
        BAD_CAST "name");
  }
  xmlXPathFreeObject(xpath);

  xpath = xmlXPathEvalExpression(
      BAD_CAST "/project/blocking/conjunction",
      xpath_ctx);

  for(i = 0; i < xpath->nodesetval->nodeNr; i++) {
    conj = conjunction_new(1);

    part_node = xpath->nodesetval->nodeTab[i]->children;

    while(part_node) {
      if(!strcmp((char *) part_node->name, "part")) {
        field_name = (char *) xmlGetProp(part_node, BAD_CAST "field-name");
        transform  = (char *) xmlGetProp(part_node, BAD_CAST "transform");
        size = (char *) xmlGetProp(part_node, BAD_CAST "size");

        conjunction_add_part(
            conj,
            field_name,
            transform,
            size ? atoi(size) : 0);

        free(field_name);
        free(transform);
        free(size);
      }
      part_node = part_node->next;
    }
    project_add_conjunction(my_proj, conj);
  }
  xmlXPathFreeObject(xpath);

  free(sep);
  xmlXPathFreeContext(xpath_ctx);
  xmlFreeDoc(doc);
  xmlCleanupParser();
}
