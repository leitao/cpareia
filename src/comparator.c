#include "comparator.h"

int
compare_has_common(record_t *r, record_t *s, int size) {
  int i, j;
  i = j = 0;

  while(i < size && j < size) {
    if(r->_keys[i] == s->_keys[j]) {
      return 1;
    } else if(r->_keys[i] < s->_keys[j]) {
      i++;
    } else {
      j++;
    }
  }
  return 0;
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

int
compare_pair(record_t *r1, record_t *r2, work_t *work) {
  size_t classes, id;
  double *scores;
  double score;
  char status, *i1, *i2;
  project_t *project;
  output_t *output;

  project = work->project;
  id = work->id;
  output = project->output;

  classes = array_size(project->classifier->comparators);
  scores = malloc(sizeof(double) * classes);

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
    output_write(output, i1, i2, status, score, scores, classes, id);

  free(scores);

  return 1;
}

void *
compare_void(void *data) {
  unsigned long int size, step;
  size_t rank, row, i, j, col, end_col, end_row, conj, tile_side;
  record_t *r1, *r2;
  comparator_pthread_params_t *par;
  project_t *project;
  work_t work;

  par = data;

  project = par->project;

  rank = par->rank;
  size = project->d0->nrows;

  work.project = project;
  work.id = rank;

  tile_side = size / (2 * par->num_threads);

  step = tile_side * par->num_threads;

  conj = array_size(project->conjunctions);

  for(row = rank * tile_side; row < size; row += step) {
    end_row = row + tile_side > size ? size : row + tile_side;

    printf("Thread %lu: %lu/%lu\n", rank, row, size);

    for(i = row; i < end_row - 1; i++) {
      r1 = array_get(project->d0->records, i);
      for(j = i + 1; j < end_row; j++) {
        r2 = array_get(project->d0->records, j);
        if(compare_has_common(r1, r2, conj))
          compare_pair(r1, r2, &work);
      }
    }

    printf("Thread %lu: Start OK\n", rank);

    for(i = row; i < end_row - 1; i++) {
      r1 = array_get(project->d0->records, i);

      for(col = row + tile_side; col < size; col += tile_side) {
        end_col = col + tile_side > size ? size : col + tile_side;

        for(j = col; j < end_col; j++) {
          end_col = col + tile_side > size ? size : col + tile_side;
          r2 = array_get(project->d0->records, j);
          if(compare_has_common(r1, r2, conj))
            compare_pair(r1, r2, &work);
        }
      }
    }
  }

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
    pthread_create(threads[i], NULL, compare_void, param);
  }

  return threads;
}
