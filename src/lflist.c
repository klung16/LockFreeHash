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
  while (true) {
    printf("%d whileeee\n", key);
    // lnode *node = lnode_new(key, val);
    lnode *found = llist_lookup(L, key);
    if (found != NULL) {
      int oldvalue = found->val;
      if (!__sync_bool_compare_and_swap(&(found->val), oldvalue, val)) continue;
      return;
    } else {
      printf("NULL\n");
    }
    node->list->mark = false;
    node->list->next = cur;

    printf("ptag: %d\n", ptag);

    llist oldval;
    llist newval;
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

}

lnode *llist_lookup(llist *L, int key) {
  int ckey;

  if (L == NULL) return NULL;

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
        // @TODO D8
        return NULL;
      }

      pmark = cmark;
      cur = next;
      ptag = ctag;
    }
  }
}

// struct list_header;
// typedef struct list_header *llist_t;
//
// struct list_node {
//   llist_t list;
//   int key;
//   int val;
// };
// typedef struct list_node *lnode_t;
//
// struct list_header {
//   lnode_t next;
//   int tag;
//   bool mark;
// };

void llist_free(llist *L) {
  llist *temp = L;
  lnode *node = temp->next;
  while (node != NULL) {
    free(temp);
    temp = node->list;
    free(node);
    node = temp->next;
    printf("wtfff\n");
  }
  free(temp);
}
