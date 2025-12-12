#include "avltree.h"


int max(int const a, int const b) {
    return (a > b) ? a : b;
}


int height(Node const * const n) {
    return n ? n->height : 0;
}


int get_balance(Node const * const n) {
    return n ? height(n->left) - height(n->right) : 0;
}


Node* new_node(int const key) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->key = key;
    node->left = node->right = NULL;
    node->height = 1;
    return node;
}


Node* rotate_right(Node * y, char (*rot)[3], int * cnt) {
    if (rot && cnt) strcpy(rot[(*cnt)++], "SR");
    Node *x = y->left;
    Node *T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}


Node* rotate_left(Node * x, char (*rot)[3], int * cnt) {
    if (rot && cnt) strcpy(rot[(*cnt)++], "SL");
    Node *y = x->right;
    Node *T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}


Node* insert(Node * node, int const key, char (*rot)[3], int * cnt) {
    if (!node) return new_node(key);

    if (key < node->key)
        node->left = insert(node->left, key, rot, cnt);
    else
        node->right = insert(node->right, key, rot, cnt);

    node->height = 1 + max(height(node->left), height(node->right));
    int balance = get_balance(node);

    if (balance > 1) {
        if (key < node->left->key) {
            return rotate_right(node, rot, cnt);
        } else {
            if (rot && cnt) strcpy(rot[(*cnt)++], "BR");
            node->left = rotate_left(node->left, NULL, NULL);
            return rotate_right(node, rot, NULL);
        }
    }
    if (balance < -1) {
        if (key > node->right->key) {
            return rotate_left(node, rot, cnt);
        } else {
            if (rot && cnt) strcpy(rot[(*cnt)++], "BL");
            node->right = rotate_right(node->right, NULL, NULL);
            return rotate_left(node, rot, NULL);
        }
    }
    return node;
}


void free_tree(Node * node) {
    if (!node) return;
    free_tree(node->left);
    free_tree(node->right);
    free(node);
}