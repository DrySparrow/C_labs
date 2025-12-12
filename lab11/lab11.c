#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int maxN = 70;
const int maxW = 1000;

int max(const int a, const int b){
    return a >= b ? a : b;
}


int main() {
    int N, W;
    if (scanf("%d %d", &N, &W) != 2)
        return 0;

    int w[maxN];   // weights
    int c[maxN];   // costs
    for (int i = 0; i < N; ++i) {
        if(scanf("%d %d", &w[i], &c[i]) != 2)
            return 0;
    }

    int * dp = calloc(maxN * maxW, sizeof(int));
    
    if (dp == NULL){
        printf("error\n");
        return 0;
    }

    for (int i = 1; i <= N; ++i){
        for (int j = 1; j <= W; ++j){
            if (j >= w[i - 1]) {
                dp[i * maxW + j] = max(dp[(i - 1) * maxW + j], dp[(i - 1) * maxW + j - w[i - 1]] + c[i - 1]);
            } else{
                dp[i * maxW + j] = dp[(i - 1) * maxW + j];
            }
        }
    }

    int selectedItems[maxN];
    int selectedPrices[maxN];

    int i = N, j = W;
    int ind_item = 0;
    while (i > 0 && j > 0) {
        if (dp[i * maxW + j] != dp[(i - 1) * maxW + j]) {
            selectedItems[ind_item] = w[i - 1];
            selectedPrices[ind_item++] = c[i - 1];
            j -= w[i - 1];
        }
        i--;
    }

    printf("%d\n", dp[N * maxW + W]);
    for (int i = ind_item - 1; i >= 0; --i)
        printf("%d %d\n", selectedItems[i], selectedPrices[i]);

    free(dp);

    return 0;
}
