#include <stdlib.h>
#include "../hdict.h"
#include "lflist.h"

struct hdict_header {
  llist_t *table;
  size_t capacity;
};
typedef struct hdict_header hdict;

int hash(hdict *H, int key) {
  return key % (H->capacity);
}

hdict *hdict_new(size_t capacity) {
  hdict *H = malloc(sizeof(hdict));
  llist_t *table = malloc(sizeof(llist_t) * capacity);
  for (int i = 0; i < capacity; i++) {
    table[i] = llist_new();
  }
  H->capacity = capacity;
  H->table = table;
  return H;
}

void hdict_insert(hdict *H, int key, int val) {
  llist_t L = H->table[hash(H, key)];
  lnode_t node = llist_lookup(L, key);
  if (node == NULL) {
    llist_insert(L, key, val);
  } else {
    node->val = val;
  }
}

void hdict_delete(hdict *H, int key) {
  llist_t L = H->table[hash(H, key)];
  llist_delete(L, key);
}

int *hdict_lookup(hdict *H, int key) {
  llist_t L = H->table[hash(H, key)];
  lnode_t node = llist_lookup(L, key);
  if (node == NULL) return NULL;
  return &(node->val);
}

void hdict_free(hdict *H) {
  for (int i = 0; i < H->capacity; i++) {
    llist_free(H->table[i]);
  }
  free(H->table);
  free(H);
}
