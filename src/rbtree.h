#ifndef _RBTREE_H_
#define _RBTREE_H_

#include <stddef.h>

typedef enum { RBTREE_RED, RBTREE_BLACK } color_t;

typedef int key_t;

typedef struct node_t {
  color_t color;
  key_t key;
  struct node_t *parent, *left, *right;
} node_t;

typedef struct {
  node_t *root;
  node_t *nil;  // for sentinel
} rbtree;

rbtree *new_rbtree(void);
void delete_rbtree(rbtree *);

node_t *rbtree_insert(rbtree *, const key_t);
node_t *rbtree_find(const rbtree *, const key_t);
node_t *rbtree_min(const rbtree *);
node_t *rbtree_max(const rbtree *);
int rbtree_erase(rbtree *, node_t *);

int rbtree_to_array(const rbtree *, key_t *, const size_t);
void right_rotate(rbtree *t,node_t *y);
void left_rotate(rbtree *t,node_t *y);
void rb_insert_fixup(rbtree *t,node_t *z);
void rb_transplant(rbtree *t, node_t *u, node_t *v);
void rbtree_delete_fixup(rbtree *t, node_t *x);
void free_node(rbtree *t,node_t *x);
void subtree_to_array(const rbtree *t, node_t *curr, key_t *arr, size_t n, size_t *count);
#endif  // _RBTREE_H_
