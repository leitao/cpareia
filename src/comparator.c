#include "comparator.h"

/*
static uint8_t
compare_diff(uint32_t *f1, uint32_t *f2) {
  uint16_t u, v;
  __m128i a1, a2;

  a1 = _mm_loadu_si128((__m128i const *)(f1));
  a2 = _mm_loadu_si128((__m128i const *)(f2));
  u = _mm_movemask_epi8(_mm_cmpeq_epi32(a1, a2));

  a1 = _mm_loadu_si128((__m128i const *)(f1 + 4));
  a2 = _mm_loadu_si128((__m128i const *)(f2 + 4));
  v = _mm_movemask_epi8(_mm_cmpeq_epi32(a1, a2));

  return !(u || v);
}*/

uint8_t
compare_skip(uint32_t *f1, uint32_t *f2, uint32_t size) {
  uint32_t i;

  for(i = 0; i < size; i++)
    if(f1[i] == f2[i]) return 0;

  return 1;
}

double
compare(comparator_t *comparator, record_t *r, record_t *s, int field) {
  int match;
  double score, freq1, freq2;
  array_t *array1, *array2;
  char *f1, *f2;
  int s1, s2;

  f1 = record_get_field(r, field);
  f2 = record_get_field(s, field);

  if(!f1 || !f2)
    return 0;

  if(comparator->exact) {
    match = !strcmp(f1, f2);
  } else {
    s1 = record_get_field_size(r, field);
    s2 = record_get_field_size(s, field);
    match = winkler(f1, f2, s1, s2) >= comparator->min_value_to_be_match;
  }

  if(match) {
    if(comparator->use_weight_table) {
      array1 = hash_get(comparator->frequency_table, f1);
      array2 = hash_get(comparator->frequency_table, f2);

      freq1 = array1 ? *(double *)array_get(array1, 0) : 0;
      freq2 = array2 ? *(double *)array_get(array2, 0) : 0;

      score = !freq1 && !freq2 ? comparator->default_weight : MIN(freq1, freq2);
    } else {
      score = comparator->log2_m_u;
    }
  } else {
    score = comparator->log2_1m_1u;
  }

  return score;
}

double
compare_all(classifier_t *classifier, record_t *r, record_t *s, double *score) {
  size_t i;
  double ret_score;

  ret_score = 0;

  for(i = 0; i < array_size(classifier->comparators); i++) {
    score[i] = compare(array_get(classifier->comparators, i), r, s, i);
    ret_score += score[i];
  }

  return ret_score;
}

void *
compare_block_void(void *data) {
  unsigned long int size, step;
  size_t rank, row, i, j, k, col, end_col, end_row, conj;
  double *scores, score;
  char status;
  record_t *r1, *r2;
  comparator_pthread_params_t *par;
  project_t *project;
  size_t classes;
  char *i1, *i2;
  output_t *output;
  block_t *block;

  par = data;

  project = par->project;
  output = project->output;

  classes = array_size(project->classifier->comparators);
  scores = malloc(sizeof(double) * classes);

  rank = par->rank;
  project = par->project;
  size = project->d0->nrows;

  step = TILE_SIDE * par->num_threads;

  conj = array_size(project->conjunctions);

  for(row = rank * TILE_SIDE; row < size; row += step) {
    end_row = row + TILE_SIDE > size ? size : row + TILE_SIDE;

    for(col = row; col < size; col += TILE_SIDE) {
      end_col = col + TILE_SIDE > size ? size : col + TILE_SIDE;

      block = block_new();

      for(i = row; i < end_row; i++) {
        r1 = array_get(project->d0->records, i);
        for(k = 0; k < conj; k++)
          block_insert(block, i, r1->_keys[k]);
      }

      for(j = col; j < end_col; j++) {
        r2 = array_get(project->d0->records, j);
        for(k = 0; k < conj; k++)
          block_insert(block, j, r2->_keys[k]);
      }

      block_free(block);
    }
  }

  /*if(i >= j || compare_skip(r1->_keys, r2->_keys, conj)) continue;
    i1 = record_get_id(r1);
    i2 = record_get_id(r2);

    score = compare_all(project->classifier, r1, r2, scores);

    if(score < project->output->min) {
    status = 'N';
    } else if(score > project->output->max) {
    status = 'Y';
    } else {
    status = '?';
    }
    if(between(score, project->output->min, project->output->max))
    output_write(output, i1, i2, status, score, scores, classes, rank);
    */

  free(scores);
  free(data);

  return NULL;
}

pthread_t **
comparator_run_async(project_t *project) {
  pthread_t **threads;
  comparator_pthread_params_t *param;
  int i;

  threads = malloc(sizeof(pthread_t *) * project->args->max_threads);

  output_open_files(project->output, project->args->max_threads);

  for(i = 0; i < project->args->max_threads; i++) {
    threads[i] = malloc(sizeof(pthread_t));
    param = malloc(sizeof(comparator_pthread_params_t));
    param->project = project;
    param->rank = i;
    param->num_threads = project->args->max_threads;
    pthread_create(threads[i], NULL, compare_block_void, param);
  }

  return threads;
}
