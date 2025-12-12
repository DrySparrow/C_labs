#include <stdio.h>
#include <stdlib.h>


typedef enum { RED, BLACK } Color;


typedef struct Node
{
    int value;
    Color color;
    struct Node *left, *right, *parent;
} Node;


typedef struct RedBlackTree
{
    Node *root;
    Node *TNULL; // Sentinel node
} RedBlackTree;


void fixInsert(RedBlackTree * tree, Node * k);


Node * createNode(const int value)
{
    Node * newNode = (Node *)malloc(sizeof(Node));
    newNode->value = value;
    newNode->color = RED; // New node is red by default
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->parent = NULL;
    return newNode;
}


RedBlackTree * createRedBlackTree(void)
{
    RedBlackTree * tree = (RedBlackTree *)malloc(sizeof(RedBlackTree));
    tree->TNULL = createNode(0); // Sentinel (nil) node
    tree->TNULL->color = BLACK;
    tree->root = tree->TNULL;
    return tree;
}


void leftRotate(RedBlackTree * tree, Node * x)
{
    Node * y = x->right;
    x->right = y->left;

    if (y->left != tree->TNULL) {
        y->left->parent = x;
    }

    y->parent = x->parent;
    if (x->parent == NULL) {
        tree->root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
        
    }
    if (y->left != tree->TNULL)
        x->right = y->left;
    y->left = x;
    x->parent = y;
}


void rightRotate(RedBlackTree * tree, Node * x)
{
    Node * y = x->left;
    x->left = y->right;

    if (y->right != tree->TNULL) {
        y->right->parent = x;
    }

    y->parent = x->parent;
    if (x->parent == NULL) {
        tree->root = y;
    } else if (x == x->parent->right) {
        x->parent->right = y;
    } else {
        x->parent->left = y;
    }
    if (y->right != tree->TNULL)
        x->left = y->right;
    y->right = x;
    x->parent = y;
}


void insert(RedBlackTree * tree, const int key)
{
    Node * newNode = createNode(key);
    newNode->left = tree->TNULL;
    newNode->right = tree->TNULL;

    Node * y = NULL;
    Node * x = tree->root;

    while (x != tree->TNULL) {
        y = x;
        if (newNode->value < x->value) {
            x = x->left;
        } else {
            x = x->right;
        }
    }

    newNode->parent = y;
    if (y == NULL) {
        tree->root = newNode; // Tree was empty
    } else if (newNode->value < y->value) {
        y->left = newNode;
    } else {
        y->right = newNode;
    }

    // Fix the tree properties
    fixInsert(tree, newNode);
}


void fixInsert(RedBlackTree * tree, Node * k)
{
    Node * u; // uncle
    while (k->parent != NULL && k->parent->color == RED) {
        if (k->parent == k->parent->parent->left) { // if k on left side
            u = k->parent->parent->right;
            if (u->color == RED) {
                // Case 1
                k->parent->color = BLACK;
                k->parent->parent->color = RED;
                u->color = BLACK;
                k = k->parent->parent;
            } else {
                // Case 2
                if (k == k->parent->right) {
                    k = k->parent;
                    leftRotate(tree, k);
                }
                k->parent->color = BLACK;
                k->parent->parent->color = RED;
                rightRotate(tree, k->parent->parent);   // why? but without it doesn't work
            }
        } else {        // if k on ritht side
            u = k->parent->parent->left;
            if (u->color == RED) {
                // Case 1
                k->parent->color = BLACK;
                u->color = BLACK;
                k->parent->parent->color = RED;
                k = k->parent->parent;
            } else {
                // Case 2
                if (k == k->parent->left) {
                    k = k->parent;
                    rightRotate(tree, k);
                }
                k->parent->color = BLACK;
                k->parent->parent->color = RED;
                leftRotate(tree, k->parent->parent);
            }
        }
        if (k == tree->root) {
            break;
        }
    }
    tree->root->color = BLACK;
}


int height_helper(const Node * node, const int cnt_black_nodes)
{
    if (node == NULL) // Sentinel check
        return cnt_black_nodes;

    return height_helper(node->right, cnt_black_nodes + (node->color == BLACK));
}


int height(const RedBlackTree * tree) {
    return height_helper(tree->root, 0);
}


void freeNode(Node * node)
{
    if ((node->left == NULL) && (node->right == NULL)){
        free(node);
    } else if (node->left != NULL){
        freeNode(node->left);
    } else{
        freeNode(node->right);
    }
}


void freeTree(RedBlackTree * tree)
{
    freeNode(tree->root);
    free(tree);
}


int main()
{
    RedBlackTree * tree = createRedBlackTree();

    int N, number;
    if (scanf("%d", &N) != 1) {
        return 0;
    }
    if (N == 0) {
        printf("%d", 0);
        return 0;
    }

    for (int i = 0; i < N; ++i) {
        if (scanf("%d", &number) != 1) {
            return 0;
        }
        insert(tree, number);
    }
    
    printf("%d", height(tree));
    freeTree(tree);
    return 0;
}
