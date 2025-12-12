#ifndef node_h
#define node_h

#include <stdio.h>
#include <stdlib.h>

#define RED 'r'
#define BLACK 'b'

typedef struct Node {
    int value;
    char color;
    struct Node* left;
    struct Node* right;
} Node;

extern Node* nodesStorage;
extern size_t currentIndex;

Node* createNode(int value);
Node* findParent(Node* root, Node* node);
Node* findGrandParent(Node* root, Node* node);
Node* insert(Node* root, int value);
Node* fixInsert(Node* root, Node* node);
Node* leftRotate(Node* root, Node* node);
Node* rightRotate(Node* root, Node* node);
int blackHeight(Node* root);

#endif
