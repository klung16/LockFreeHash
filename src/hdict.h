#ifndef _HDICT_H_
#define _HDICT_H_

typedef struct hdict_header* hdict;

// create new hash table
hdict_t hdict_new(size_t capacity);

// hash function
int hash(hdict_t H, int key);

// insert (key, val) pair into hash table
void hdict_insert(hdict_t H, int key, int val);

// delete key from hash table
void hdict_delete(hdict_t H, int key);

// if key in hash table return value, else return NULL
int *hdict_lookup(hdict_t H, int key);

// frees hash table
void hdict_free(hdict_t H);

#endif
