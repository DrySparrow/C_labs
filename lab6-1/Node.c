#include "Node.h"

Node* createNode(int const value) {
    Node* node = &nodesStorage[currentIndex++];
    node->value = value;
    node->color = RED;
    node->left = NULL;
    node->right = NULL;
    return node;
}

Node* findParent(Node* const root, Node* const node) {
    if (root == NULL || root == node)
        return NULL;
    Node* current = root;
    Node* parent = NULL;
    while (current != NULL) {
        if (node->value < current->value) {
            parent = current;
            current = current->left;
        } else if (node->value > current->value) {
            parent = current;
            current = current->right;
        } else {
            break;
        }
    }
    return parent;
}

Node* findGrandParent(Node* const root, Node* const node) {
    if (root == NULL || root == node)
        return NULL;
    Node* current = root;
    Node* grandparent = NULL;
    Node* parent = NULL;
    
    while (current != NULL) {
        if (node->value < current->value) {
            grandparent = parent;
            parent = current;
            current = current->left;
        } else if (node->value > current->value) {
            grandparent = parent;
            parent = current;
            current = current->right;
        } else {
            break;
        }
    }
    return grandparent;
}

Node* leftRotate(Node* root, Node* x) {
    Node* y = x->right;
    if (y == NULL) return root;
    x->right = y->left;
    y->left = x;

    Node* parentX = findParent(root, x);
    if (parentX == NULL) {
        root = y;
    } else {
        if (parentX->left == x) parentX->left = y;
        else parentX->right = y;
    }
    return root;
}

Node* rightRotate(Node* root, Node* y) {
    Node* x = y->left;
    if (x == NULL) return root;
    y->left = x->right;
    x->right = y;

    Node* parentY = findParent(root, y);
    if (parentY == NULL) {
        root = x;
    } else {
        if (parentY->left == y) parentY->left = x;
        else parentY->right = x;
    }
    return root;
}

Node* fixInsert(Node* root, Node* node) {
    Node* current = node;
    Node* grandparent = findGrandParent(root, current);
    Node* parent;
    if (grandparent != NULL && (grandparent->left != NULL) && (grandparent->left->left == current || grandparent->left->right == current))
        parent = grandparent->left;
    else if (grandparent != NULL && (grandparent->right != NULL))
        parent = grandparent->right;
    else
        parent = findParent(root, current);
    
    // Node* parent = findParent(root, current);
    // Node* grandparent = parent ? findParent(root, parent) : NULL;

    while (parent != NULL && parent->color == RED && grandparent != NULL) {
        Node* uncle = (parent == grandparent->left) ? grandparent->right : grandparent->left;

        if (uncle && uncle->color == RED) {
            // Case 1: Uncle is RED
            parent->color = BLACK;
            uncle->color = BLACK;
            grandparent->color = RED;
            
            current = grandparent;
            // parent = findParent(root, current);
            // grandparent = parent ? findParent(root, parent) : NULL;
            grandparent = findGrandParent(root, current);
            if (grandparent != NULL && (grandparent->left != NULL) && (grandparent->left->left == current || grandparent->left->right == current))
                parent = grandparent->left;
            else if (grandparent != NULL && (grandparent->right != NULL))
                parent = grandparent->right;
            else
                parent = findParent(root, current);

        } else {
            // Case 2/3: Uncle is BLACK
            if (parent == grandparent->left) {
                if (current == parent->right) {
                    // Case 2: Left-Right
                    root = leftRotate(root, parent);
                    Node* temp = parent;
                    parent = current;
                    current = temp;
                }
                // Case 3: Left-Left
                parent->color = BLACK;
                grandparent->color = RED;
                root = rightRotate(root, grandparent);
            } else {
                if (current == parent->left) {
                    // Case 2: Right-Left
                    root = rightRotate(root, parent);
                    Node* temp = parent;
                    parent = current;
                    current = temp;
                }
                // Case 3: Right-Right
                parent->color = BLACK;
                grandparent->color = RED;
                root = leftRotate(root, grandparent);
            }
            break;
        }
    }
    root->color = BLACK;
    
    return root;
}

Node* insert(Node* root, int const value) {
    Node* newNode = createNode(value);
    if (root == NULL) {
        newNode->color = BLACK;
        return newNode;
    }
    Node* current = root;
    Node* parent = NULL;
    while (current != NULL) {
        parent = current;
        if (value < current->value)
            current = current->left;
        else
            current = current->right;
    }
    if (value < parent->value)
        parent->left = newNode;
    else
        parent->right = newNode;
    return fixInsert(root, newNode);
}

int blackHeight(Node* const root) {
    int height = 1;
    Node* current = root;
    while (current != NULL) {
        if (current->color == BLACK)
            height++;
        current = current->left;
    }
    return height;
}
