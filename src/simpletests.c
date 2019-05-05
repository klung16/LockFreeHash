#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <assert.h>
#include <stdbool.h>
#include <omp.h>

#include "hdict.h"
#include "util/cycletimer.h"
#include "util/util.h"

#define NUM_BUCKETS 10000
#define NUM_TEST_VALUES 100000
#define RAND_KEY_SEED 0
#define RAND_VAL_SEED 17

static void usage() {
  char *use_string = "[-s SEQ]";
  outmsg("Usage: %s\n", use_string);
  outmsg("   -h               Print this message\n");
  outmsg("   -s               Turn on sequential test\n");
  done();
  exit(0);
}

bool contains(int* keys, int val, int i) {
  int j;
  for (j = 0; j < i; j++) {
    if (keys[j] == val) {
      return true;
    }
  }
  return false;
}

hdict_t setup(int* keys, int* values) {
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
  // return NULL;
  return hdict_new(NUM_BUCKETS);
}

void test_seq_setup(hdict_t dict, int* keys) {
  int i;
  assert(dict != NULL);

  for (i = 0; i < NUM_TEST_VALUES; i++) {
    hdict_delete(dict, keys[i]);
  }

  for (i = 0; i < NUM_TEST_VALUES; i++) {
    assert(hdict_lookup(dict, keys[i]) == NULL);
  }
}

void test_seq_insert(hdict_t dict, int* keys, int* values) {
  int actual, expected, i;

  for (i = 0; i < NUM_TEST_VALUES; i++) {
    hdict_insert(dict, keys[i], values[i]);
  }

  for (i = 0; i < NUM_TEST_VALUES; i++) {
    actual = *hdict_lookup(dict, keys[i]);
    assert(actual == values[i]);
  }

  // Test that inserting overwrites existing values
  for (i = 0; i < NUM_TEST_VALUES/2; i++) {
    hdict_insert(dict, keys[i], values[NUM_TEST_VALUES - i - 1]);
  }

  for (i = 0; i < NUM_TEST_VALUES; i++) {
    actual = *hdict_lookup(dict, keys[i]);
    if (i < NUM_TEST_VALUES/2) {
      expected = values[NUM_TEST_VALUES - i - 1];
    } else {
      expected = values[i];
    }

    assert(actual == expected);
  }
}

void test_seq_delete(hdict_t dict, int* keys, int* values) {
  int i, actual;

  for (i = 0; i < NUM_TEST_VALUES/2; i++) {
    hdict_delete(dict, keys[i]);
  }

  for (i = 0; i < NUM_TEST_VALUES; i++) {
    if (i < NUM_TEST_VALUES/2) {
      assert(hdict_lookup(dict, keys[i]) == NULL);
    } else {
      actual = *hdict_lookup(dict, keys[i]);
      assert(actual == values[i]);
    }
  }

  for (i = 0; i < NUM_TEST_VALUES; i++) {
    hdict_delete(dict, keys[i]);
  }
  for (i = 0; i < NUM_TEST_VALUES; i++) {
    assert(hdict_lookup(dict, keys[i]) == NULL);
  }
}

void test_par_setup(hdict_t dict, int* keys) {
  int i;
  assert(dict != NULL);
#if OMP
  #pragma omp parallel for
  for (i = 0; i < NUM_TEST_VALUES; i++) {
    hdict_delete(dict, keys[i]);
  }

  #pragma omp parallel for
  for (i = 0; i < NUM_TEST_VALUES; i++) {
    assert(hdict_lookup(dict, keys[i]) == NULL);
  }
#endif
}

void test_par_insert(hdict_t dict, int* keys, int* values) {
  int actual, expected, i;

  #pragma omp parallel for
  for (i = 0; i < NUM_TEST_VALUES; i++) {
    hdict_insert(dict, keys[i], values[i]);
  }

  #pragma omp parallel for
  for (i = 0; i < NUM_TEST_VALUES; i++) {
    actual = *hdict_lookup(dict, keys[i]);
    assert(actual == values[i]);
  }

  // Test that inserting overwrites existing values
  #pragma omp parallel for
  for (i = 0; i < NUM_TEST_VALUES/2; i++) {
    hdict_insert(dict, keys[i], values[NUM_TEST_VALUES - i - 1]);
  }

  #pragma omp parallel for
  for (i = 0; i < NUM_TEST_VALUES; i++) {
    actual = *hdict_lookup(dict, keys[i]);
    if (i < NUM_TEST_VALUES/2) {
      expected = values[NUM_TEST_VALUES - i - 1];
    } else {
      expected = values[i];
    }

    assert(actual == expected);
  }

}

void test_par_delete(hdict_t dict, int* keys, int* values) {
  int i, actual;

  #pragma omp parallel for
  for (i = 0; i < NUM_TEST_VALUES/2; i++) {
    hdict_delete(dict, keys[i]);
  }

  #pragma omp parallel for
  for (i = 0; i < NUM_TEST_VALUES; i++) {
    if (i < NUM_TEST_VALUES/2) {
      assert(hdict_lookup(dict, keys[i]) == NULL);
    } else {
      actual = *hdict_lookup(dict, keys[i]);
      assert(actual == values[i]);
    }
  }

  #pragma omp parallel for
  for (i = 0; i < NUM_TEST_VALUES; i++) {
    hdict_delete(dict, keys[i]);
  }

  #pragma omp parallel for
  for (i = 0; i < NUM_TEST_VALUES; i++) {
    assert(hdict_lookup(dict, keys[i]) == NULL);
  }
}

int main(int argc, char *argv[])
{
  int c;
  bool seq = false;
  double start_time, delta_time;

  fprintf(stdout, "Starting simple correctness tests... \n");

  /* Grabs Arguments */
  char *optstring = "hs";
  while ((c = getopt(argc, argv, optstring)) != -1) {
    switch(c) {
      case 'h':
        usage();
        break;
      case 's':
        seq = true;
        break;
      default:
        outmsg("Unknown option '%c'\n", c);
        usage();
        done();
        exit(1);
    }
  }

  int *keys = malloc(sizeof(int) * NUM_TEST_VALUES);
  int *values = malloc(sizeof(int) * NUM_TEST_VALUES);
  hdict_t dict = setup(keys, values);

  // Sequential Correctness Tests
  if (seq) {
    outmsg("    Starting simple sequential correctness test... \n");
    start_time = currentSeconds();
    test_seq_setup(dict, keys);
    test_seq_insert(dict, keys, values);
    test_seq_delete(dict, keys, values);
    delta_time = currentSeconds() - start_time;
    outmsg("    Complete! Took %f secs\n", delta_time);
    hdict_free(dict);
  }

  // Parallel Correctness Tests
#if OMP
  // omp_set_num_threads(6);
  dict = setup(keys, values);
  outmsg("    Starting simple parallel correctness test... \n");
  start_time = currentSeconds();

  test_par_setup(dict, keys);
  test_par_insert(dict, keys, values);
  test_par_delete(dict, keys, values);

  delta_time = currentSeconds() - start_time;
  outmsg("    Complete! Took %f secs\n", delta_time);

#endif

  hdict_free(dict);
  free(keys);
  free(values);

  outmsg("Tests complete! Exiting...\n");

  return 1;
}