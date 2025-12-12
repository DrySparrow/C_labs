#ifndef avltree_h
#define avltree_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>     // for strcpy

#include "Node.h"

int max(int const a, int const b);
int height(Node const * const n);
int get_balance(Node const * const n);
Node* new_node(int const key);
Node* rotate_right(Node * y, char (*rot)[3], int * cnt);
Node* rotate_left(Node * x, char (*rot)[3], int * cnt);
Node* insert(Node * node, int key, char (*rot)[3], int * cnt);
void free_tree(Node * node);

#endif