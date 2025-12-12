Node* fixInsert(Node* root, Node* node) {
    Node* current = node;
    // Первоначально находим parent и grandparent
    Node* parent = findParent(root, current);
    Node* grandparent = parent ? findParent(root, parent) : NULL;

    while (parent != NULL && parent->color == RED && grandparent != NULL) {
        Node* uncle = (parent == grandparent->left) ? grandparent->right : grandparent->left;

        if (uncle != NULL && uncle->color == RED) {
            // Case 1: Uncle is RED
            parent->color = BLACK;
            uncle->color = BLACK;
            grandparent->color = RED;
            
            // Перемещаемся вверх по дереву
            current = grandparent;
            parent = findParent(root, current);
            grandparent = parent ? findParent(root, parent) : NULL;
        } else {
            // Case 2/3: Uncle is BLACK
            if (parent == grandparent->left) {
                if (current == parent->right) {
                    // Case 2: Left-Right
                    root = leftRotate(root, parent);
                    // Обновляем указатели после поворота
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
            
            // После поворота grandparent становится ребенком, выходим из цикла
            break;
        }
    }
    
    // Всегда проверяем корень
    if (root != NULL && root->color == RED) {
        root->color = BLACK;
    }
    return root;
}