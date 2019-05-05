#include <stdlib.h>
#include <immintrin.h> 
#include <stdint.h>

#include "../hdict.h"
#include "rtmlist.h"

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
  return H;}

void hdict_insert(hdict *H, int key, int val) {
  lnode_t node;
  llist_t L = H->table[hash(H, key)];
  
  unsigned status = _xbegin();
  if (status == _XBEGIN_STARTED) {
    if (L->is_stopped) {
      _xabort((uint8_t)(status));
    } else {
      node = llist_lookup(L, key);
      if (node == NULL) {
        llist_insert(L, key, val);
      } else {
        node->val = val;
      }
    }
    _xend();
  } else {
    pthread_mutex_lock(&(L->mutex));
    L->is_stopped = true;
    node = llist_lookup(L, key);
    if (node == NULL) {
      llist_insert(L, key, val);
    } else {
      node->val = val;
    }
    L->is_stopped = false;
    pthread_mutex_unlock(&(L->mutex));
  }
}

void hdict_delete(hdict *H, int key) {
  llist_t L = H->table[hash(H, key)];
  
  unsigned status = _xbegin();
  if (status == _XBEGIN_STARTED) {
    if (L->is_stopped) {
      _xabort((uint8_t)(status));
    } else {
      llist_delete(L, key);
    }
    _xend();
  } else {
    pthread_mutex_lock(&(L->mutex));
    L->is_stopped = true;
    llist_delete(L, key);
    L->is_stopped = false;
    pthread_mutex_unlock(&(L->mutex));
  }
}

int *hdict_lookup(hdict *H, int key) {
  lnode_t node;
  llist_t L = H->table[hash(H, key)];
  
  unsigned status = _xbegin();
  if (status == _XBEGIN_STARTED) {
    if (L->is_stopped) {
      _xabort((uint8_t)(status));
    } else {
      node = llist_lookup(L, key);
    }
    _xend();
  } else {
    pthread_mutex_lock(&(L->mutex));
    L->is_stopped = true;
    node = llist_lookup(L, key);
    L->is_stopped = false;
    pthread_mutex_unlock(&(L->mutex));
  }
  
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