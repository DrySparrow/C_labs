#include <stdio.h>
#include <stdlib.h>

#include "Node.h"

#define MAXSIZE 2000000
#define NORMALSIZE 10000

Node* nodesStorage = NULL;
size_t currentIndex = 0;

int main(void) {
    int N, value;
    if (scanf("%d", &N) != 1)
        return 0;
    if (N == 0) {
        printf("0");
        return 0;
    }
    if (N < NORMALSIZE)
        nodesStorage = (Node*)malloc(sizeof(Node) * NORMALSIZE);
    else
        nodesStorage = (Node*)malloc(sizeof(Node) * MAXSIZE);
    if (nodesStorage == NULL)
        return 0;
    
    Node* root = NULL;
    for (int i = 0; i < N; ++i) {
        if (scanf("%d", &value) != 1) {
            free(nodesStorage);
            return 0;
        }
        root = insert(root, value);
    }
    printf("%d", blackHeight(root));
    free(nodesStorage);
    return 0;
}
