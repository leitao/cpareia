#include "comparator.h"

double
first_of(double v1, double v2, double v3) {
  return v1 ? v1 : (v2 ? v2 : v3);
}

int
compare_exact(char *field1, char *field2) {
  return !strcmp(field1, field2);
}

int
compare_approx(char *f1, char *f2, compare_fn fn, double min_value) {
  return fn(f1, f2) >= min_value;
}

double
compare(char *f1, char *f2, double inverse_freq_f1, double inverse_freq_f2, double default_weight, int exact, double min_value, double m, double u, int use_weight_table) {
  int match;
  double note;

  if(!f1 || !f2) {
    return 0;
  }

  if(exact) {
    match = compare_exact(f1, f2);
  } else {
    match = compare_approx(f1, f2, winkler,  min_value);
  }

  if(match) {
    if(use_weight_table) {
      note = first_of(inverse_freq_f1, inverse_freq_f2, default_weight);
    } else {
      note = log2(m / u);
    }
  } else {
    note = log2(1.0 - m / 1.0 - u);
  }

  return note;
}
