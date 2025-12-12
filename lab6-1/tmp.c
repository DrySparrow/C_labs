void fixIncert(Node *node, int *root, int *path, int path_len) {
    while (path_len >= 3) {
        int z = path[path_len-1];
        int p = path[path_len-2];
        int g = path[path_len-3];
        int u = (p == tree->nodes[g].left) ? tree->nodes[g].right : tree->nodes[g].left;
        if (get_color(tree, p) == BLACK) break;
        if (get_color(tree, u) == RED) {
            set_color(tree, p, BLACK);
            set_color(tree, u, BLACK);
            set_color(tree, g, RED);
            path_len -= 2;
        } else {
            if (p == tree->nodes[g].left) {
                if (z == tree->nodes[p].right) {
                    rotate_left(tree, root, p, g);
                    int tmp = z;
					z = p;
					p = tmp;
                }
                set_color(tree, p, BLACK);
                set_color(tree, g, RED);
                rotate_right(tree, root, g, path_len >= 4 ? path[path_len-4] : NULL);
            } else {
                if (z == tree->nodes[p].left) {
                    rotate_right(tree, root, p, g);
                    int tmp = z; z = p; p = tmp;
                }
                set_color(tree, p, BLACK);
                set_color(tree, g, RED);
                rotate_left(tree, root, g, path_len >= 4 ? path[path_len-4] : NULL);
            }
            break;
        }
    }
    set_color(tree, *root, BLACK);
}