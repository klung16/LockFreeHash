#include <stdlib.h>
#include "lflist.h"

typedef struct list_node lnode;
typedef struct list_header llist;

llist *prev;
bool pmark, cmark;
lnode *cur, *next;
int ptag, ctag;

llist *llist_new() {
  llist *L = malloc(sizeof(llist));
  L->next = NULL;
  L->tag = 0;
  L->mark = false;
  return L;
}

lnode *lnode_new(int key, int val) {
  lnode *node = malloc(sizeof(lnode));
  node->key = key;
  node->val = val;
  node->list = llist_new();
  return node;
}

void llist_insert(llist *L, int key, int val) {
  lnode *node = lnode_new(key, val);
  llist oldval, newval;
  while (true) {
    // lnode *node = lnode_new(key, val);
    lnode *found = llist_lookup(L, key);
    if (found != NULL) {
      int oldvalue = found->val;
      if (!__sync_bool_compare_and_swap(&(found->val), oldvalue, val)) continue;
      return;
    } else {
    }
    node->list->mark = false;
    node->list->next = cur;

    oldval.mark = false;
    oldval.next = cur;
    oldval.tag = ptag;
    newval.mark = false;
    newval.next = node;
    newval.tag = ptag+1;
    if (__sync_bool_compare_and_swap((long long*)prev, *((long long*)&oldval), *((long long*)&newval))) return;
    // free(node);
  }
}

void llist_delete(llist *L, int key) {
  llist oldval1, newval1;
  llist oldval, newval;
  while (true) {
    if (llist_lookup(L, key) == NULL) return;

    oldval1.mark = false;
    oldval1.next = next;
    oldval1.tag = ctag;
    newval1.mark = true;
    newval1.next = next;
    newval1.tag = ctag+1;
    if (!__sync_bool_compare_and_swap((long long*)(cur->list), *((long long*)&oldval1), *((long long*)&newval1))) {
      continue;
    }

    oldval.mark = false;
    oldval.next = cur;
    oldval.tag = ptag;
    newval.mark = false;
    newval.next = next;
    newval.tag = ptag+1;
    if (__sync_bool_compare_and_swap((long long*)prev, *((long long*)&oldval), *((long long*)&newval))) {
      //@TODO FREE LIST?
      //free(cur);
    } else {
      llist_lookup(L, key);
    }

  }
}

lnode *llist_lookup(llist *L, int key) {
  if (L == NULL) return NULL;

  int ckey;
  llist oldval, newval;

  while (true) {
    prev = L;
    pmark = prev->mark;
    cur = prev->next;
    ptag = prev->tag;

    while (true) {
      if (cur == NULL) return NULL;
      cmark = cur->list->mark;
      next = cur->list->next;
      ctag = cur->list->tag;

      ckey = cur->key;

      // llist oldval;
      // oldval.mark = false;
      // oldval.next = cur;
      // oldval.tag = ptag;

      if (prev->mark != false || prev->next != cur || prev->tag != ptag) break;
      //@TODO: Fix ABA
      //if (!__sync_bool_compare_and_swap((long long*)prev, *((long long*)&oldval), *((long long*)&oldval))) break;

      if (!cmark) {
        if (ckey > key) {
          return NULL;
        } else if (ckey == key) {
          return cur;
        }
        prev = cur->list;
      } else {
        // printf("aloha\n");
        // oldval.mark = false;
        // oldval.next = cur;
        // oldval.tag = ptag;
        // newval.mark = false;
        // newval.next = next;
        // newval.tag = ptag+1;
        // if (__sync_bool_compare_and_swap((long long*)prev, *((long long*)&oldval), *((long long*)&newval))) {
        //   //@TODO FREE LIST?
        //   // free(cur);
        //   ctag = ptag+1;
        // } else {
        //   break;
        // }
        return NULL;

      }

      pmark = cmark;
      cur = next;
      ptag = ctag;
    }
  }
}

void llist_free(llist *L) {
  llist *temp = L;
  lnode *node = temp->next;
  while (node != NULL) {
    free(temp);
    temp = node->list;
    free(node);
    node = temp->next;
    // printf("wtfff\n");
  }
  free(temp);
}
