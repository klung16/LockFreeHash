#include <stdlib.h>
#include <stdbool.h>
#include "lflist.h"

typedef struct list_node lnode;
typedef struct list_header llist;

lnode *lnode_new(int key, int val) {
  lnode *node = malloc(sizeof(lnode));
  node->next = NULL;
  node->key = key;
  node->val = val;
  return node;
}

llist *llist_new() {
  llist *L = malloc(sizeof(llist));
  L->head = lnode_new(0, 0);
  L->tail = lnode_new(0, 0);
  L->head->next = L->tail;
  return L;
}

bool is_marked_reference(lnode *node) {
  return (((unsigned long) node) & 1) == 1;
}

lnode *get_unmarked_reference(lnode *node) {
  return (lnode *) ((((unsigned long) node) >> 1) << 1);
}

lnode *get_marked_reference(lnode *node) {
  return (lnode *) (((unsigned long) node) | 1);
}

lnode *llist_search(llist *L, int key, lnode **left_node) {
  lnode *left_node_next, *right_node;
  lnode *t, *t_next;

  do {
    t = L->head;
    t_next = L->head->next;

    // find left and right node
    do {
      if (!is_marked_reference(t_next)) {
        *left_node = t;
        left_node_next = t_next;
      }

      t = get_unmarked_reference(t_next);
      if (t == L->tail) break;
      t_next = t->next;
    } while (is_marked_reference(t_next) || t->key < key);
    right_node = t;

    // check nodes are adjacent
    if (left_node_next == right_node) {
      if (right_node != L->tail && is_marked_reference(right_node->next)) continue;
      else return right_node;
    }

    // remove one or more marked nodes
    if (__sync_bool_compare_and_swap(&((*left_node)->next), left_node_next, right_node)) {
      if (right_node != L->tail && is_marked_reference(right_node->next)) continue;
      else return right_node;
    }

  } while (true);
}

void llist_insert(llist *L, int key, int val) {
  lnode *new_node = lnode_new(key, val);
  lnode *right_node, *left_node;

  do {
    right_node = llist_search(L, key, &left_node);
    // @TODO update value when key same
    if ((right_node != L->tail) && (right_node->key == key)) return;
    new_node->next = right_node;
    if (__sync_bool_compare_and_swap(&(left_node->next), right_node, new_node)) return;
  } while (true);
}

void llist_delete(llist *L, int key) {
  lnode *right_node, *right_node_next, *left_node;

  do {
    right_node = llist_search(L, key, &left_node);
    if ((right_node == L->tail) || (right_node->key != key)) return;
    right_node_next = right_node->next;
    if (!is_marked_reference(right_node_next)) {
      if (__sync_bool_compare_and_swap(&(right_node->next), right_node_next, get_marked_reference(right_node_next))) break;
    }
  } while (true);

  if (!__sync_bool_compare_and_swap(&(left_node->next), right_node, right_node_next)) {
    right_node = llist_search(L, right_node->key, &left_node);
    // @TODO else free
  }
  return;
}

lnode *llist_lookup(llist *L, int key) {
  lnode *right_node, *left_node;

  right_node = llist_search(L, key, &left_node);
  if (right_node == L->tail || right_node->key != key) return NULL;
  return right_node;
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
