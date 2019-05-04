#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include "lfbacklist.h"

typedef struct list_node lnode;
typedef struct list_header llist;

/***** Helper Function Declartions ****/

bool is_marked(lnode *node);
bool is_flagged(lnode *node);
lnode *get_ptr(lnode *node);
lnode *set_ptr(lnode *node, unsigned long mark, unsigned long flag);

void help_marked(lnode *prev_node, lnode *del_node);
void help_flagged(lnode *prev_node, lnode *del_node);
void try_mark(lnode *del_node);
bool try_flag(lnode *prev_node, lnode *target_node, lnode **res_node);
void llist_search(int key, lnode *start_node, lnode **res1_node, lnode **res2_node);
void llist_search2(int key, lnode *start_node, lnode **res1_node, lnode **res2_node);

/***** Helper Function Implementations ****/

bool is_marked(lnode *node) {
  return (((unsigned long) node) & 1) == 1;
}

bool is_flagged(lnode *node) {
  return (((unsigned long) node) & 2) == 1;
}

lnode *get_ptr(lnode *node) {
  return (lnode *) ((((unsigned long) node) >> 2) << 2);
}

lnode *set_ptr(lnode *node, unsigned long mark, unsigned long flag) {
  return (lnode *) ((unsigned long) node | flag << 1 | mark);
}

void help_marked(lnode *prev_node, lnode *del_node) {
  lnode *next_node = get_ptr(del_node->next);
  __sync_bool_compare_and_swap(&(prev_node->next), set_ptr(del_node, 0, 1), set_ptr(next_node, 0, 0));
}

void help_flagged(lnode *prev_node, lnode *del_node) {
  del_node->prev = prev_node;
  if (!is_marked(del_node)) try_mark(del_node);
  help_marked(prev_node, del_node);
}

void try_mark(lnode *del_node) {
  lnode *next_node, *result;
  while (!is_marked(del_node->next)) {
    next_node = get_ptr(del_node->next);
    result = __sync_val_compare_and_swap(&(del_node->next), set_ptr(next_node, 0, 0), set_ptr(next_node, 1, 0));
    if (!is_marked(result) && is_flagged(result)) help_flagged(del_node, get_ptr(result));
  }
}

bool try_flag(lnode *prev_node, lnode *target_node, lnode **res_node) {
  lnode *target_node_unflagged, *target_node_flagged, *result, *del_node;

  while (true) {
    target_node_unflagged = set_ptr(target_node, 0, 0);
    target_node_flagged = set_ptr(target_node, 0, 1);

    if (prev_node->next == target_node_flagged) {
      *res_node = prev_node;
      return false;
    }
    result = __sync_val_compare_and_swap(&(prev_node->next), target_node_unflagged, target_node_flagged);
    if (result == target_node_unflagged) {
      *res_node = prev_node;
      return true;
    } else if (result == target_node_flagged) {
      *res_node = prev_node;
      return false;
    }

    while (is_marked(prev_node->next)) { prev_node = prev_node->prev; }
    llist_search2(target_node->key, prev_node, &prev_node, &del_node);
    if (del_node != target_node) {
      *res_node = NULL;
      return false;
    }
  }
}

void llist_search(int key, lnode *start_node, lnode **res1_node, lnode **res2_node) {
  lnode *curr_node = start_node;
  lnode *next_node = get_ptr(curr_node->next);

  while (next_node->key <= key) {

    while (is_marked(next_node) && (!is_marked(curr_node) || get_ptr(curr_node->next) != next_node)) {
      if (get_ptr(curr_node->next) == next_node) help_marked(curr_node, next_node);
      next_node = get_ptr(curr_node->next);
    }

    if (next_node->key <= key) {
      curr_node = next_node;
      next_node = get_ptr(curr_node->next);
    }
  }

  *res1_node = curr_node;
  *res2_node = next_node;
}

void llist_search2(int key, lnode *start_node, lnode **res1_node, lnode **res2_node) {
  lnode *curr_node = start_node;
  lnode *next_node = get_ptr(curr_node->next);

  while (next_node->key < key) {

    while (is_marked(next_node) && (!is_marked(curr_node) || get_ptr(curr_node->next) != next_node)) {
      if (get_ptr(curr_node->next) == next_node) help_marked(curr_node, next_node);
      next_node = get_ptr(curr_node->next);
    }

    if (next_node->key < key) {
      curr_node = next_node;
      next_node = get_ptr(curr_node->next);
    }
  }

  *res1_node = curr_node;
  *res2_node = next_node;
}

/***** Interface Function Implementations ****/

lnode *lnode_new(int key, int val) {
  lnode *node = malloc(sizeof(lnode));
  node->next = NULL;
  node->key = key;
  node->val = val;
  return node;
}

llist *llist_new() {
  llist *L = malloc(sizeof(llist));
  L->head = lnode_new(INT_MIN, 0);
  L->head->next = lnode_new(INT_MAX, 0);
  return L;
}

void llist_insert(llist *L, int key, int val) {
  lnode *prev_node, *next_node;
  llist_search(key, L->head, &prev_node, &next_node);
  // @TODO UPDATE VALUE
  if (prev_node->key == key) return;
  lnode *new_node = lnode_new(key, val);

  lnode *prev_node_next, *result;
  while (true) {
    prev_node_next = prev_node->next;
    if (is_flagged(prev_node_next)) help_flagged(prev_node, get_ptr(prev_node_next->next));
    else {
      new_node->next = set_ptr(next_node, 0, 0);
      result = __sync_val_compare_and_swap(&(prev_node->next), set_ptr(next_node, 0, 0), set_ptr(new_node, 0, 0));
      if (result == set_ptr(next_node, 0, 0)) return;
      else {
        if (!is_marked(result) && is_flagged(result)) help_flagged(prev_node, get_ptr(result->next));
        while (is_marked(prev_node)) { prev_node = prev_node->prev; }
      }
    }

    llist_search(key, prev_node, &prev_node, &next_node);
    if (prev_node->key == key) {
      free(new_node);
      return;
    }
  }
}

void llist_delete(llist *L, int key) {
  lnode *prev_node, *del_node;
  llist_search2(key, L->head, &prev_node, &del_node);
  if (del_node->key != key) return;
  bool result = try_flag(prev_node, del_node, &prev_node);
  if (prev_node != NULL) help_flagged(prev_node, del_node);
  if (!result) return;
  return;
}

lnode *llist_lookup(llist *L, int key) {
  lnode *curr_node, *next_node;

  llist_search(key, L->head, &curr_node, &next_node);
  if (curr_node->key != key) return NULL;
  return curr_node;
}

void llist_free(llist *L) {
  lnode *node = L->head;
  lnode *temp;
  while (node != NULL) {
    temp = node;
    node = get_ptr(node->next);
    free(temp);
  }
  free(L);
}
