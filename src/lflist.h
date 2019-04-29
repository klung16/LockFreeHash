#ifndef _LFLIST_H_
#define _LFLIST_H_

#include <stdbool.h>

struct list_header;
typedef struct list_header *llist_t;

struct list_node {
  llist_t list;
  int key;
  int val;
};
typedef struct list_node *lnode_t;

struct list_header {
  lnode_t next;
  int tag;
  bool mark;
};

// create new linked list node
lnode_t lnode_new(int key, int val);

// create new linked list
llist_t llist_new();

// insert (key, val) pair into linked list
void llist_insert(llist_t L, int key, int val);

// delete key from linked list
void llist_delete(llist_t L, int key);

// if key in linked list return node, else return NULL
lnode_t llist_lookup(llist_t L, int key);

// frees linked list
void llist_free(llist_t L);

#endif
