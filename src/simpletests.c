/*
 * Simple correctness testing  
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "hdict.h"

#define NUM_BUCKETS 100
#define NUM_TEST_VALUES 10000
#define RAND_KEY_SEED 0
#define RAND_VAL_SEED 17

hdict_t setup(int* keys, int* values) {
  int i;
  
  // Set up the keys 
  srand(RAND_KEY_SEED);
  for (i = 0; i < NUM_TEST_VALUES; i++) {
    if (i <= NUM_TEST_VALUES/2) {
      keys[i] = rand();
    // Negative keys 
    } else {
      keys[i] = -rand();
    }
  }
  
  // Set up the values 
  srand(RAND_VAL_SEED);
  for (i = 0; i < NUM_TEST_VALUES; i++) {
    if (i <= (0.25)*NUM_TEST_VALUES || i > (0.75)*NUM_TEST_VALUES) {
      values[i] = rand();
    // Negative keys 
    } else {
      values[i] = -rand();
    }
  }
  
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

int main(int argc, char *argv[])
{
	 int keys[NUM_TEST_VALUES];
   int values[NUM_TEST_VALUES];
   hdict_t dict; 
   
   // Sequential Correctness Tests 
   dict = setup(keys, values); 
   test_seq_setup(dict, keys);
   test_seq_insert(dict, keys, values);
   test_seq_delete(dict, keys, values);
   hdict_free(dict);
   
   // TODO: Parallel Correctness Tests
   
   return 1;
}
