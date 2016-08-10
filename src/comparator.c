#include "comparator.h"

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
}

double
compare(comparator_t *comparator, record_t *r, record_t *s, int field) {
  int match;
  double score, freqf1, freqf2;
  array_t *array1, *array2;
  char *f1, *f2;
  int s1, s2;

  f1 = record_get_field(r, field);
  f2 = record_get_field(s, field);

  if(!f1 || !f2) {
    return 0;
  }

  if(comparator->exact) {
    match = !strcmp(f1, f2);
  } else {
    s1 = record_get_field_size(r, field);
    s2 = record_get_field_size(s, field);
    match = winkler(f1, f2, s1, s2) >= comparator->min_value_to_be_match;
  }

  if(match) {
    if(comparator->use_weight_table) {
      /*
       * Implementar tabela de peso
       * score = inverse_freq_f1 | inverse_freq_f2 | default_weight
       */
      array1 = hash_get(comparator->frequency_table, f1);
      array2 = hash_get(comparator->frequency_table, f2);

      freqf1 = array1 ? *(double *)array_get(array1, 0) : 0;
      freqf2 = array2 ? *(double *)array_get(array2, 0) : 0;

      /* We need to get the smalest inverse frequency, since it means
       * this element is more frequent.
       */
      if(!freqf1 && !freqf2) {
        score = comparator->default_weight;
      } else {
        score = MIN(freqf1, freqf2);
      }

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
  size_t rank, row, i, j, col, end_col, end_row;
  double *scores, score;
  char status;
  record_t *r1, *r2;
  comparator_pthread_params_t *par;
  project_t *project;
  size_t classes;
  char *i1, *i2;
  output_t *output;

  par = data;

  project = par->project;
  output = project->output;

  classes = array_size(project->classifier->comparators);
  scores = malloc(sizeof(double) * classes);

  rank = par->rank;
  project = par->project;
  size = project->d0->nrows;

  step = TILE_SIDE * par->num_threads;

  for(row = rank * TILE_SIDE; row < size; row += step) {
    end_row = row + TILE_SIDE > size ? size : row + TILE_SIDE;

    for(col = row; col < size; col += TILE_SIDE) {
      end_col = col + TILE_SIDE > size ? size : col + TILE_SIDE;

      for(i = row; i < end_row; i++) {
        r1 = array_get(project->d0->records, i);
        i1 = record_get_id(r1);
        for(j = col; j < end_col; j++) {
          r2 = array_get(project->d0->records, j);
          i2 = record_get_id(r2);

          if(i >= j || compare_diff(r1->_keys, r2->_keys)) continue;

          score = compare_all(project->classifier, r1, r2, scores);

          /*
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
        }
      }
    }
  }

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
