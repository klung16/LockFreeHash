#include <stdlib.h>
#include "hohlist.h"

typedef struct list_node lnode;
typedef struct list_header llist;

lnode *lnode_new(int key, int val) {
  return NULL;
}

llist *llist_new() {
  return NULL;
}

void llist_insert(llist *L, int key, int val) {
  return;
}

void llist_delete(llist *L, int key) {
  return;
}

lnode *llist_lookup(llist *L, int key) {
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
