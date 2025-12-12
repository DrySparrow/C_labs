#include <stdio.h>
#include <stdlib.h>

typedef enum { RED, BLACK } Color;

typedef struct Node
{
    int value;
    Color color;
    struct Node *left, *right;
} Node;

typedef struct RedBlackTree
{
    Node *root;
} RedBlackTree;


void fixInsert(RedBlackTree * tree, Node * k);


Node * createNode(int value)
{
    Node * newNode = (Node *)malloc(sizeof(Node));
    newNode->value = value;
    newNode->color = RED; // New node is red by default
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}


RedBlackTree * createRedBlackTree(void)
{
    RedBlackTree * tree = (RedBlackTree *)malloc(sizeof(RedBlackTree));
    tree->root = NULL;
    return tree;
}


Node * findParent(Node * root, const int childValue) {
    if (root == NULL)
        return NULL;
    if ((root->left && root->left->value == childValue) || (root->right && root->right->value == childValue))
        return root;

    Node * leftSearch = findParent(root->left, childValue);
    if (leftSearch != NULL)
        return leftSearch;

    Node * rightSearch = findParent(root->right, childValue);
    if (rightSearch != NULL)
        return rightSearch;

    return NULL;
}


void leftRotate(RedBlackTree * tree, Node * x)
{
    Node * y = x->right;
    x->right = y->left;

    Node * x_parent = findParent(tree->root, x->value);
    if (x_parent == NULL) {
        tree->root = y;
    } else if (x == x_parent->left) {
        x_parent->left = y;
    } else {
        x_parent->right = y;
        
    }
    if (y->left != NULL)
        x->right = y->left;
    y->left = x;
}


void rightRotate(RedBlackTree * tree, Node * x)
{
    Node * y = x->left;
    x->left = y->right;

    Node * x_parent = findParent(tree->root, x->value);
    if (x_parent == NULL) {
        tree->root = y;
    } else if (x == x_parent->right) {
        x_parent->right = y;
    } else {
        x_parent->left = y;
    }
    if (y->right != NULL)
        x->left = y->right;
    y->right = x;
}


void insert(RedBlackTree * tree, const int key)
{
    Node * newNode = createNode(key);
    newNode->left = NULL;
    newNode->right = NULL;

    Node * y = NULL;
    Node * x = tree->root;

    while (x != NULL) {
        y = x;
        if (newNode->value < x->value) {
            x = x->left;
        } else {
            x = x->right;
        }
    }

    if (y == NULL) {
        tree->root = newNode; // Tree was empty
    } else if (newNode->value < y->value) {
        y->left = newNode;
    } else {
        y->right = newNode;
    }

    fixInsert(tree, newNode);
}


void fixInsert(RedBlackTree * tree, Node * k)
{
    Node * u; // uncle
    Node * k_parent = findParent(tree->root, k->value);
    Node * k_parent_parent;

    while (k_parent != NULL && k_parent->color == RED) {
        k_parent = findParent(tree->root, k->value);
        k_parent_parent = findParent(tree->root, k_parent->value);

        if (k_parent == k_parent_parent->left) { // if k on left side
            u = k_parent_parent->right;
            if (u->color == RED) {
                // Case 1
                k_parent->color = BLACK;
                k_parent_parent->color = RED;
                u->color = BLACK;
                k = k_parent_parent;
            } else {
                // Case 2
                if (k == k_parent->right) {
                    k = k_parent;
                    leftRotate(tree, k);
                }
                k_parent->color = BLACK;
                k_parent_parent->color = RED;
                rightRotate(tree, k_parent_parent);
            }
        } else {        // if k on ritht side
            u = k_parent_parent->left;
            if (u->color == RED) {
                // Case 1
                k_parent->color = BLACK;
                u->color = BLACK;
                k_parent_parent->color = RED;
                k = k_parent_parent;
            } else {
                // Case 2
                if (k == k_parent->left) {
                    k = k_parent;
                    rightRotate(tree, k);
                }
                k_parent->color = BLACK;
                k_parent_parent->color = RED;
                leftRotate(tree, k_parent_parent);
            }
        }
        if (k == tree->root) {
            break;
        }
    }
    tree->root->color = BLACK;
}


int height_helper(Node * node, int cnt_black_nodes)
{
    if (node == NULL) // Sentinel check
        return cnt_black_nodes + 1;

    return height_helper(node->right, cnt_black_nodes + (node->color == BLACK));
}

int height(RedBlackTree * tree) {
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
