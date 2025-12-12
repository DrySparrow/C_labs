#include <stdio.h>
#include <stdlib.h>

#include "graph.h"
#include "stack.h"


int main() {
    int N;
    int M;
    
    if(scanf("%d", &N) != 1) {
        puts("bad number of lines");
        return 0;
    }
    if (!(N >= 0 && N <= 2000)){
        puts("bad number of vertices");
        return 0;
    }
    
    if(scanf("%d", &M) != 1) {
        puts("bad number of lines");
        return 0;
    }
    if (!(M >= 0 && M <= N * (N - 1) / 2)) {
        puts("bad number of edges");
        return 0;
    }
    
    Graph* g = graph_init(N);
    
    for (int i = 0; i < M; ++i) {
        int src;
        int dst;
        if(scanf("%d %d", &src, &dst) != 2) {
            graph_destroy(g);
            puts("bad number of lines");
            return 0;
        }
        if(!(src > 0 && src <= N) || !(dst > 0 && dst <= N)) {
            graph_destroy(g);
            puts("bad vertex");
            return 0;
        }
        
        graph_add_connection(g, src - 1, dst - 1);
    }
    
    Stack* result = NULL;
    if(graph_get_order(g, &result) < 0) {
        graph_destroy(g);
        puts("impossible to sort");
        return 0;
    }
    
    int val;
    while(!stack_pop(result, &val)) {
        printf("%d ", val + 1);
    }
    
    stack_free(result);
    graph_destroy(g);
    
    return 0;
}