#include "comparator.h"

double
compare(comparator_t *comparator, record_t *r, record_t *s, int field) {
  int match;
  double score;
  char *f1, *f2;

  f1 = record_get_field(r, field);
  f2 = record_get_field(s, field);

  if(!f1 || !f2) {
    return 0;
  }

  if(comparator->exact) {
    match = !strcmp(f1, f2);
  } else {
    match = winkler(f1, f2) >= comparator->min_value_to_be_match;
  }

  if(match) {
    if(comparator->use_weight_table) {
      /*
       * Implementar tabela de peso
       * score = inverse_freq_f1 | inverse_freq_f2 | default_weight
       */
      score = comparator->default_weight;
    } else {
      score = comparator->log2_m_u;
    }
  } else {
    score = comparator->log2_1m_1u;
  }

  return score;
}

double
compare_all(
    classifier_t *classifier,
    record_t *r1,
    record_t *r2,
    double *scores) {
  size_t i;
  double score;

  score = 0;

  for(i = 0; i < array_size(classifier->comparators); i++) {
    scores[i] = compare(array_get(classifier->comparators, i), r1, r2, i);
    score += scores[i];
  }

  return score;
}

void
compare_block(array_t *array, project_t *project) {
  size_t i, j, size, classes;
  record_t *r1, *r2;
  result_t *result;
  double *scores;
  double score;
  char status;

  size = array_size(array);
  classes = array_size(project->classifier->comparators);

  for(i = 0; i < size - 1; i++) {
    r1 = array_get(array, i);
    for(j = i + 1; j < size; j++) {
      r2 = array_get(array, j);
      scores = malloc(sizeof(double) * classes);
      score = compare_all(project->classifier, r1, r2, scores);

      if(score < project->output->min) {
        status = 'N';
      } else if(score > project->output->max) {
        status = 'Y';
      } else {
        status = '?';
      }
      result = result_new(r1, r2, status, score, scores, classes);
      output_push(project->output, result);
    }
  }
}

void
compare_block_void(void *array, void *project) {
  compare_block(array, project);
}

void
comparator_get_block(gpointer key, gpointer array, gpointer pool) {
  (void) key;

  pool_push((pool_t *) pool, array);
}

void
comparator_run(project_t *project, int num_threads) {
  pool_t *pool;

  pool = pool_new(num_threads, project, compare_block_void);

  hash_foreach(project->blocks, comparator_get_block, pool);

  pool_free(pool);
}
