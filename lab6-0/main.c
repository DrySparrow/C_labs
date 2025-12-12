#include "avltree.c"

#define MAX_SIZE 2000000

int main(void)
{
    int N;
    if (scanf("%d\n", &N) != 1) {
        printf("Empty input");
        return 0;
    }
    
    if (N < 0 || N > 2000000) {
        printf("Bad input");
        return 0;
    }
    
    int * nums = (int*)malloc(MAX_SIZE * sizeof(int));
    if (nums == NULL)
        return 0;
        
    for (int i = 0; i < N; ++i) {
        if (scanf("%d", &nums[i]) != 1) {
            printf("Bad input\n");
            free(nums);
            return 0;
        }
    }
    
    int extra;
    if (scanf("%d", &extra) == 1) {
        free(nums);
        printf("Bad input\n");
        return 0;
    }
    
    Node *root = NULL;
    char rotations[100][3];
    int rot_cnt = 0;
    
    for (int i = 0; i < N; i++) {
        if (N < 100) {
            root = insert(root, nums[i], rotations, &rot_cnt);
        } else {
            root = insert(root, nums[i], NULL, NULL);
        }
    }
    
    if (N < 100) {
        for (int i = 0; i < rot_cnt; i++) {
            printf("%s ", rotations[i]);
        }
    }
    printf("%d\n", height(root));
    
    free(nums);
    free_tree(root);
    return 0;
}