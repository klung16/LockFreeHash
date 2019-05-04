#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <getopt.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <omp.h>

#include "hdict.h"
#include "cycletimer.h"
#include "util.h"

#define DEFAULT_LOAD_FACTOR 10
#define DEFAULT_NUM_TEST_VALUES 1000000
#define DEFAULT_DELETE_RATIO 0.1
#define DEFAULT_INSERT_RATIO 0.1
#define DEFAULT_NUM_THREADS 4

#define DEFAULT_RAND_KEY_SEED 0
#define DEFAULT_RAND_VAL_SEED 17
#define RATIO 100

static void usage() {
  char *use_string = "[-n NUM_OPS] [-l LOAD_FACTOR] [-d DELETE_RATIO] [-i INSERT_RATIO] [-t THD]";
  outmsg("Usage: %s\n", use_string);
  outmsg("   -h               Print this message\n");
  outmsg("   -n NUM_OPS       Number of operations (Default to %d)\n", DEFAULT_NUM_TEST_VALUES);
  outmsg("   -l LOAD_FACTOR   Load factor (number of data points / number of buckets)\n");
  outmsg("   -d DELETE_RATIO  Delete ratio for total number of operations [%]\n");
  outmsg("   -i INSERT_RATIO  Insert ratio for total number of operations [%]\n");
  outmsg("                        0% <= DELETE_RATIO + INSERT_RATIO <= 100%\n");
  outmsg("   -t THD    Set number of threads\n");
  done();
  exit(0);
}

hdict_t setup(int* keys, int* values, int num_ops, int load_factor) {
  int i, num_buckets;

  // Sets up the keys
  srand(DEFAULT_RAND_KEY_SEED);
  for (i = 0; i < num_ops; i++) {
    keys[i] = rand();
  }
  
  // Sets up the values
  srand(DEFAULT_RAND_VAL_SEED);
  for (i = 0; i < num_ops; i++) {
    values[i] = rand();
  }
  
  num_buckets = round(num_ops / load_factor);
  
  outmsg("      The hash table is initilized with %d buckets.\n", num_buckets);
  
  return hdict_new(num_buckets);
}

void test_insert(hdict_t dict, int* keys, int* values, int num_insert) {
  int i;
  #pragma omp parallel for
  for (i = 0; i < num_insert; i++) {
    hdict_insert(dict, keys[i], values[i]);
  }
}

void test_lookup(hdict_t dict, int* keys, int* values, int num_search) {
  int i;
  #pragma omp parallel for
  for (i = 0; i < num_search; i++) {
    hdict_lookup(dict, keys[i]);
  }
}

void test_delete(hdict_t dict, int* keys, int* values, int num_delete) { 
  int i;
  #pragma omp parallel for
  for (i = 0; i < num_delete; i++) {
    hdict_delete(dict, keys[i]);
  }
}

int main(int argc, char *argv[]) {
  int c;
  int num_ops = DEFAULT_NUM_TEST_VALUES;
  int load_factor = DEFAULT_LOAD_FACTOR;
  double delete_ratio = DEFAULT_DELETE_RATIO;
  double insert_ratio = DEFAULT_INSERT_RATIO;
  int num_threads = DEFAULT_NUM_THREADS;
  double search_ratio, write_ratio;
  double start_time, delta_time;
  hdict_t dict;
  int num_insert, num_search, num_delete;
  
  /* Grabs Arguments */
  char *optstring = "hn:l:d:i:t:";
  while ((c = getopt(argc, argv, optstring)) != -1) {
    switch(c) {
      case 'h':
        usage();
  	    break;
      case 'n':
        num_ops = atoi(optarg);
        break;
      case 'l':
        load_factor = atoi(optarg);
        break;
      case 'd':
        sscanf(optarg, "%lf", &delete_ratio);
        break;
      case 'i':
        sscanf(optarg, "%lf", &insert_ratio);
        break;
      case 't':
        num_threads = atoi(optarg);
        break;
      default:
        outmsg("Unknown option '%c'\n", c);
  	    usage();
  	    done();
  	    exit(1);
    }
  }
  
  /* Checks Legality */
  if (num_ops <= 0) {
    outmsg("NUM_OPS must be positive.\n");
    usage();
    done();
    exit(1);
  }
  if (load_factor <= 0) {
    outmsg("LOAD_FACTOR must be positive.\n");
    usage();
    done();
    exit(1);
  }
  write_ratio = delete_ratio + insert_ratio;
  
  if (write_ratio < 0.0 || write_ratio > 1.0) {
    outmsg("DELETE_RATIO + INSERT_RATIO = %lf not in range.\n", write_ratio);
    usage();
    done();
    exit(1);
  }
  if (num_threads <= 0) {
    outmsg("THD must be positive.\n");
    usage();
    done();
    exit(1);
  }
  
  search_ratio = (1 - write_ratio);
  
  outmsg("  Running the program with %d operations, load factor = %d, delete ratio = %.2f, insert ratio = %.2f, search ratio = %.2f\n", 
            num_ops, load_factor, delete_ratio, insert_ratio, search_ratio);
  outmsg("  Running with %d threads.  Max possible is %d.\n", num_threads, omp_get_max_threads());
  omp_set_num_threads(num_threads);
  
  /* Set up */
  outmsg("  The program is setting up the data...");
  num_insert = round(num_ops * insert_ratio);
  num_search = round(num_ops * search_ratio);
  num_delete = round(num_ops * delete_ratio);
  int *keys = malloc(sizeof(int) * num_ops);
  int *values = malloc(sizeof(int) * num_ops);
  dict = setup(keys, values, num_ops, load_factor);
  outmsg("  Set up complete!");
  
  start_time = currentSeconds();
  
  test_insert(dict, keys, values, num_insert);
  test_lookup(dict, keys, values, num_search);
  test_delete(dict, keys, values, num_delete);
  
  delta_time = currentSeconds() - start_time;
  fprintf(stdout, "Complete! Took %f secs\n", delta_time);
  
  hdict_free(dict);
  free(keys);
  free(values);
  fprintf(stdout, "Tests complete! Exiting...\n");
  
  return 0;
}