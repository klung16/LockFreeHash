#include <stdlib.h>
#include "lflist.h"

typedef struct list_node lnode;
typedef struct list_header llist;

lnode *lnode_new(int key, int val) {
  lnode *node = malloc(sizeof(lnode));
  node->key = key;
  node->val = val;
  node->next = NULL;
  return node;
}

llist *llist_new() {
  llist *L = malloc(sizeof(llist));
  L->head = NULL;
  return L;
}

void llist_insert(llist *L, int key, int val) {
  lnode *node = lnode_new(key, val);
  node->next = L->head;
  L->head = node;
}

void llist_delete(llist *L, int key) {
  lnode *node = L->head;
  if (node == NULL) return;

  if (node->key == key) {
    L->head = node->next;
    free(node);
    return;
  }
  lnode *prev = L->head;
  for (node = L->head->next; node != NULL; node = node->next) {
    if (node->key == key) {
      prev->next = node->next;
      free(node);
      return;
    }
    prev = node;
  }
}

lnode *llist_lookup(llist *L, int key) {
  for (lnode *node = L->head; node != NULL; node = node->next) {
    if (node->key == key) return node;
  }
  return NULL;
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
