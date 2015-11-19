#include "project.h"

project_t *
project_new() {
  project_t *project;

  project = malloc(sizeof(project_t));

  project->conjunctions = array_new_prealloc(1);

  project->blocks = hash_new();

  return project;
}

void
project_print(project_t *project) {
  size_t i;

  printf("Project: %s\n", project->name);
  printf("Task: %s\n", project->task);
  for(i = 0; i < array_size(project->conjunctions); i++) {
    printf("Conjunction %d:\n", (int) i);
    conjunction_print(array_get(project->conjunctions, i));
  }

  printf("\nClassifier\n");
  classifier_print(project->classifier);

  printf("\nD0:\n");
  database_print(project->d0);
}

void
project_free(project_t *project) {
  size_t i;

  free(project->task);
  free(project->name);
  database_free(project->d0);

  for(i = 0; i < array_size(project->conjunctions); i++) {
    conjunction_free(array_get(project->conjunctions, i));
  }

  hash_free(project->blocks);
  array_free(project->conjunctions);
  classifier_free(project->classifier);
  free(project);
}

void
project_add_conjunction(project_t *project, conjunction_t *conjunction) {
  array_append(project->conjunctions, conjunction);
}

void
project_parse_classifier(project_t *project, xmlXPathContextPtr ctx) {
  xmlXPathObjectPtr xpath;
  xmlChar *match_min, *not_match_max, *m, *u, *missing, *function;
  xmlChar *field1;
  xmlChar *min_value_to_be_match, *use_weight_table, *frequency_table;
  int exact;
  char *child_name;
  xmlNodePtr child;
  comparator_t *comparator;

  xpath = xmlXPathEvalExpression(
      BAD_CAST "/project/classifier",
      ctx);

  match_min = xmlGetProp(
      xpath->nodesetval->nodeTab[0],
      BAD_CAST "match-min-result");

  not_match_max = xmlGetProp(
      xpath->nodesetval->nodeTab[0],
      BAD_CAST "not-match-max-result");

  project->classifier = classifier_new(
      atoi((char *) match_min),
      atoi((char *) not_match_max));

  child = xpath->nodesetval->nodeTab[0]->children;

  while(child) {
    child_name = (char *) child->name;

    m = xmlGetProp(child, BAD_CAST "m");
    u = xmlGetProp(child, BAD_CAST "u");
    missing = xmlGetProp(child, BAD_CAST "missing");
    min_value_to_be_match = xmlGetProp(child, BAD_CAST "minValueToBeMatch");
    use_weight_table = xmlGetProp(child, BAD_CAST "use-weight-table");
    frequency_table = xmlGetProp(child, BAD_CAST "frequency-table");
    function = xmlGetProp(child,BAD_CAST "function");
    field1 = xmlGetProp(child, BAD_CAST "field1");

    if(!strcmp(child_name, "exact-string-comparator")) {
      exact = 1;
    } else if(!strcmp(child_name, "approx-string-comparator")) {
      exact = 0;
    } else {
      exact = -1;
    }

    if(exact != -1) {
      comparator = comparator_new(
          exact,
          use_weight_table ? (!strcmp((char *) use_weight_table, "true") ? 1 : 0) : 0,
          m ? atof((char *) m) : 0,
          u ? atof((char *) u) : 0,
          missing ? atof((char *) missing) : 0,
          project_get_field_id(project, (char *) field1),
          (char *) frequency_table,
          (char *) function,
          min_value_to_be_match ? atof((char *) min_value_to_be_match) : 0);
      classifier_add_comparator(project->classifier, comparator);
    }

    free(m);
    free(u);
    free(field1);
    free(missing);
    free(min_value_to_be_match);

    child = child->next;
  }

  xmlXPathFreeObject(xpath);

  free(match_min);
  free(not_match_max);
}

void
project_parse_datasource(project_t *project, xmlXPathContextPtr ctx) {
  xmlXPathObjectPtr xpath;
  xmlChar *filename, *sep, *rows;
  int i;

  xpath = xmlXPathEvalExpression(
      BAD_CAST "/project/data-sources/data-source[@id=0]",
      ctx);

  filename = xmlGetProp(
      xpath->nodesetval->nodeTab[0],
      BAD_CAST "file");

  sep = xmlGetProp(
      xpath->nodesetval->nodeTab[0],
      BAD_CAST "field-separator");

  rows = xmlGetProp(
      xpath->nodesetval->nodeTab[0],
      BAD_CAST "rows");

  xmlXPathFreeObject(xpath);

  xpath = xmlXPathEvalExpression(
      BAD_CAST "/project/data-sources/data-source[@id=0]/fields/field",
      ctx);

  if(rows) {
    project->d0 = database_new(
        xpath->nodesetval->nodeNr,
        strtoull((char *)rows, NULL, 10));
  } else {
    handle_error("Missing attribute 'rows'\n");
  }

  project->d0->filename = (char *) filename;

  if(!strcmp((char *) sep, "\\t")) {
    project->d0->sep = '\t';
  } else {
    project->d0->sep = *sep;
  }

  for(i = 0; i < xpath->nodesetval->nodeNr; i++) {
    project->d0->fields[i] = xmlGetProp(
        xpath->nodesetval->nodeTab[i],
        BAD_CAST "name");
  }

  xmlXPathFreeObject(xpath);
  free(sep);
  free(rows);
}

void
project_parse_project(project_t *project, xmlXPathContextPtr ctx) {
  xmlXPathObjectPtr xpath;

  xpath = xmlXPathEvalExpression(BAD_CAST "/project", ctx);

  project->name = xmlGetProp(
      xpath->nodesetval->nodeTab[0],
      BAD_CAST "name");

  project->task = xmlGetProp(
      xpath->nodesetval->nodeTab[0],
      BAD_CAST "task");

  if(strcmp((char *)project->task, "deduplication")) {
    handle_error("We only support 'deduplication' right now");
  }

  xmlXPathFreeObject(xpath);
}

void
project_parse_conjunctions(project_t *project, xmlXPathContextPtr ctx) {
  xmlXPathObjectPtr xpath;
  xmlNode *part_node;
  xmlChar *field_name, *transform, *size;
  conjunction_t *conjunction;
  int i, field;

  xpath = xmlXPathEvalExpression(
      BAD_CAST "/project/blocking/conjunction",
      ctx);

  for(i = 0; i < xpath->nodesetval->nodeNr; i++) {
    conjunction = conjunction_new(1);

    part_node = xpath->nodesetval->nodeTab[i]->children;

    while(part_node) {
      if(!strcmp((char *) part_node->name, "part")) {
        field_name = xmlGetProp(part_node, BAD_CAST "field-name");
        transform  = xmlGetProp(part_node, BAD_CAST "transform");
        size = xmlGetProp(part_node, BAD_CAST "size");

        field = project_get_field_id(project, (char *) field_name);
        assert(field != -1);

        conjunction_add_part(
            conjunction,
            field,
            (char *) transform,
            size ? atoi((char *) size) : 0);

        free(field_name);
        free(transform);
        free(size);
      }
      part_node = part_node->next;
    }
    project_add_conjunction(project, conjunction);
  }

  array_fini(project->conjunctions);

  xmlXPathFreeObject(xpath);
}

void
project_parse(project_t *project, char *file_name) {
  xmlDocPtr doc;
  xmlXPathContextPtr xpath_ctx;

  xmlInitParser();

  if((doc = xmlParseFile(file_name)) == NULL)
    handle_error("Unable to parse XML");

  if((xpath_ctx = xmlXPathNewContext(doc)) == NULL)
    handle_error("Unable to create XPath");

  project_parse_project(project, xpath_ctx);
  project_parse_datasource(project, xpath_ctx);
  project_parse_conjunctions(project, xpath_ctx);
  project_parse_classifier(project, xpath_ctx);

  xmlXPathFreeContext(xpath_ctx);
  xmlFreeDoc(doc);
  xmlCleanupParser();
}

int
project_get_field_id(project_t *project, char *field_name) {
  unsigned int i;
  int field;

  field = -1;

  for(i = 0; i < project->d0->num_fields; i++) {
    if(!strcmp((char *) field_name, (char *) project->d0->fields[i])) {
      field = i;
      break;
    }
  }

  assert(field != -1);

  return field;
}
