#include "result.h"

result_t *
result_new(
    record_t *r1,
    record_t *r2,
    char status,
    double score,
    double *scores,
    int num_scores) {

  result_t *result;

  result = malloc(sizeof(result_t));

  result->r1 = r1;
  result->r2 = r2;
  result->status = status;
  result->score = score;
  result->scores = scores;
  result->num_scores = num_scores;

  return result;
}

void
result_free(result_t *result) {
  free(result->scores);
  free(result);
}
