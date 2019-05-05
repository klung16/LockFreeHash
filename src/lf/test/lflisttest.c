/*
 * Simple lock-free list correctness testing
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <omp.h>

#include "../lflist.h"
#include "../../util/cycletimer.h"

#define NUM_TEST_VALUES 50000
#define RAND_KEY_SEED 0
#define RAND_VAL_SEED 17

bool contains(int* keys, int val, int i) {
  int j;
  for (j = 0; j < i; j++) {
    if (keys[j] == val) {
      return true;
    }
  }
  return false;
}

void setup(int* keys, int* values) {
  int i, val;

  // Set up the keys
  srand(RAND_KEY_SEED);
  for (i = 0; i < NUM_TEST_VALUES; i++) {
    if (i <= NUM_TEST_VALUES/2) {
      val = rand();
    } else {
      // Negative keys
      val = -rand();
    }

    while (contains(keys, val, i)) {
      val = rand();
    }
    keys[i] = val;
  }

  // Set up the values
  srand(RAND_VAL_SEED);
  for (i = 0; i < NUM_TEST_VALUES; i++) {
    if (i <= (0.25)*NUM_TEST_VALUES || i > (0.75)*NUM_TEST_VALUES) {
      values[i] = rand();
    } else {
      // Negative keys
      values[i] = -rand();
    }
  }
  return;
}


void simple_lf_test(int* keys, int* values) {
#if OMP
  // omp_set_num_threads(6);
  llist_t list = llist_new();
  #pragma omp parallel for
  for (int i = 0; i < NUM_TEST_VALUES; i++) {
    llist_insert(list, keys[i], values[i]);
  }

  #pragma omp barrier

  #pragma omp parallel for
  for (int i = 0; i < NUM_TEST_VALUES; i++) {
    lnode_t actual = llist_lookup(list, keys[i]);
    assert(actual->val == values[i]);
  }
  llist_free(list);
#endif
}

int main(int argc, char *argv[]) {
  double start_time, delta_time;
  int *keys = malloc(sizeof(int) * NUM_TEST_VALUES);
  int *values = malloc(sizeof(int) * NUM_TEST_VALUES);
  setup(keys, values);

  fprintf(stdout, "Starting simple parallel lf-list correctness test... \n");
  start_time = currentSeconds();
  simple_lf_test(keys, values);
  delta_time = currentSeconds() - start_time;
  fprintf(stdout, "Complete! Took %f secs\n", delta_time);

  free(keys);
  free(values);
  fprintf(stdout, "Complete! \n");
  return 1;
}
