#include "result.h"

result_t *
result_new(
    char *id1,
    char *id2,
    char status,
    double score,
    double *scores,
    int num_scores) {

  result_t *result;

  result = malloc(sizeof(result_t));

  result->id1 = id1;
  result->id2 = id2;
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
