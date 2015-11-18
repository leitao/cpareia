#include "classifier.h"

/*<approx-string-comparator use-weight-table="true" m="0.9261" u="0.0188"
missing="0.94" field1="nomecompleto" frequency-table="tb_freq_nome_paciente"
function="winkler" minValueToBeMatch="0.95"/>
*/

comparator_t *
comparator_new(
    int exact,
    int use_weight_table,
    double m,
    double u,
    double missing,
    int field1,
    char *frequency_table,
    char *function,
    double min_value_to_be_match) {
  comparator_t *comparator;

  comparator = malloc(sizeof(comparator_t));

  comparator->exact = exact;
  comparator->use_weight_table = use_weight_table;
  comparator->m = m;
  comparator->u = u;
  comparator->missing = missing;
  comparator->field1 = field1;
  comparator->frequency_table = strdup(frequency_table);
  comparator->function = strdup(function);
  comparator->min_value_to_be_match = min_value_to_be_match;

  return comparator;
}

void
comparator_free(comparator_t *comparator) {
  free(comparator->function);
  free(comparator->frequency_table);
  free(comparator);
}

classifier_t *
classifier_new(int match_min, int not_match_max) {
  classifier_t *classifier;
  classifier = malloc(sizeof(classifier_t));

  classifier->match_min = match_min;
  classifier->not_match_max = not_match_max;

  return classifier;
}

void
classifier_free(classifier_t *classifier) {
  free(classifier);
}

void
classifier_print(classifier_t *classifier) {
  printf("match_min: %d\n", classifier->match_min);
  printf("not_match_max: %d\n", classifier->not_match_max);
}
