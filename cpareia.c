#include "cpareia.h"

int
main(int argc,  char *argv[]) {
  xmlDocPtr doc;
  xmlXPathContextPtr xpathCtx;
  xmlXPathObjectPtr xpathObj;
  char task[] = { "/project" };
  xmlChar *uri;

  xmlInitParser();

  if(argc != 2)
    handle_error("argc");

  doc = xmlParseFile(argv[1]);

  if (doc == NULL)
    handle_error("Unable to parse XML");

  xpathCtx = xmlXPathNewContext(doc);

  if(xpathCtx == NULL)
    handle_error("Unable to create XPath");

  xpathObj = xmlXPathEvalExpression(BAD_CAST task, xpathCtx);

  if(xpathObj == NULL)
    handle_error("Unable to evaluate XPath");

  uri = xmlGetProp(xpathObj->nodesetval->nodeTab[0], BAD_CAST "task");
  printf("Task: %s\n", uri);
  xmlFree(uri);

  xmlXPathFreeObject(xpathObj);
  xmlXPathFreeContext(xpathCtx);
  xmlFreeDoc(doc);
  xmlCleanupParser();
  return 0;
}
