#ifndef _LOCKLIST_H_
#define _LOCKLIST_H_

typedef struct list_node *lnode_t;
struct list_node {
  lnode_t next;
  int key;
  int val;
};

struct list_header {
  lnode_t head;
};
typedef struct list_header *llist_t;

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
