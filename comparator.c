#include "comparator.h"

size_t num = 0;

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

double *
compare_all(classifier_t *classifier, record_t *r1, record_t *r2) {
  double *score;
  size_t i, size;

  size = array_size(classifier->comparators);
  score = malloc(sizeof(double) * size);

  record_print(r1);
  record_print(r2);

  for(i = 0; i < size; i++) {
    score[i] = compare(array_get(classifier->comparators, i), r1, r2, i);
    printf("score=%f\n", score[i]);
  }

  return score;
}

void
compare_block(array_t *array, project_t *project) {
  size_t i, j, size;
  record_t *r1, *r2;

  size = array_size(array);

  for(i = 0; i < size - 1; i++) {
    r1 = array_get(array, i);
    for(j = i + 1; j < size; j++) {
      r2 = array_get(array, j);
      free(compare_all(project->classifier, r1, r2));
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
  num++;

  pool_push((pool_t *) pool, array);
}

void
comparator_start(project_t *project) {
  pool_t *pool;

  pool = pool_new(1, project, compare_block_void);

  hash_foreach(project->blocks, comparator_get_block, pool);

  pool_free(pool);

  printf_magenta("acabei com num=%lu\n", num);
}
