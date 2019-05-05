#include <stdlib.h>
#include <limits.h>
#include "hohlist.h"

typedef struct list_node lnode;
typedef struct list_header llist;

lnode *lnode_new(int key, int val) {
  lnode *node = malloc(sizeof(lnode));
  node->next = NULL;
  node->key = key;
  node->val = val;
  pthread_mutex_init(&(node->mutex), NULL);
  return node;
}

llist *llist_new() {
  llist *L = malloc(sizeof(llist));
  L->head = lnode_new(INT_MIN, 0);
  L->head->next = lnode_new(INT_MAX, 0);
  return L;
}

void llist_search(llist *L, int key, lnode **prev_ptr, lnode **cur_ptr) {
  lnode *prev, *cur, *old_prev;

  prev = L->head;
  pthread_mutex_lock(&(prev->mutex));
  cur = prev->next;
  pthread_mutex_lock(&(cur->mutex));

  while (key < cur->key) {
    old_prev = prev;
    prev = cur;
    cur = cur->next;
    pthread_mutex_unlock(&(old_prev->mutex));
    pthread_mutex_lock(&(cur->mutex));
  }

  *prev_ptr = prev;
  *cur_ptr = cur;
}

void llist_insert(llist *L, int key, int val) {
  lnode *node = lnode_new(key, val);
  lnode *prev, *cur;
  llist_search(L, key, &prev, &cur);

  node->next = cur;
  prev->next = node;
  pthread_mutex_unlock(&(prev->mutex));
  pthread_mutex_unlock(&(cur->mutex));
}

void llist_delete(llist *L, int key) {
  lnode *prev, *cur;
  llist_search(L, key, &prev, &cur);

  lnode *del = NULL;
  if (key == cur->key) {
    prev->next = cur->next;
    del = cur;
  }
  pthread_mutex_unlock(&(prev->mutex));
  pthread_mutex_unlock(&(cur->mutex));

  if (del != NULL) free(del);
}

lnode *llist_lookup(llist *L, int key) {
  lnode *prev, *cur;
  llist_search(L, key, &prev, &cur);
  return cur;
}

void llist_free(llist *L) {
  lnode *node = L->head;
  lnode *temp;
  while (node != NULL) {
    temp = node;
    node = node->next;
    free(temp);
  }
  free(L);
}
