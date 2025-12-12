#include <stdlib.h>

#include "bit_set.h"
#include "graph.h"

struct Graph {
    bit_set* connections;
    int node_count;
};

Graph* graph_init(int const nodes) {
    Graph* g = malloc(sizeof(Graph));
    if (g == NULL)
        exit(0);
    g->connections = bit_set_new(nodes * nodes);
    g->node_count = nodes;
    
    return g;
}

void graph_destroy(Graph* g) {
    if(g) {
        bit_set_free(g->connections);
        free(g);
    }
}

int graph_add_connection(Graph* g, int src, int dest) {
    return bit_set_set(g->connections, src * g->node_count + dest);
}

int graph_has_connection(Graph* g, int const src, int dest) {
    return bit_set_get(g->connections, src * g->node_count + dest);
}

static int dfs_cycle_check(Graph* g, int* states, int node) {
    states[node] = 1;
    for (int i = 0; i < g->node_count; ++i) {
        if(bit_set_get(g->connections, node * g->node_count + i) > 0) {
            if(states[i] == 1 || (states[i] == 0 && !dfs_cycle_check(g, states, i))) {
                return 0;
            }
        }
    }
    states[node] = 2;

    return 1;
}

int is_acyclic_graph(Graph * g) {
    int* states = calloc(g->node_count, sizeof(int));
    if (states == NULL)
        exit(0);
    int result = 1;
    
    for (int i = 0; i < g->node_count; ++i) {
        if(!states[i] && !dfs_cycle_check(g, states, i)) {
            result = 0;
            break;
        }
    }
    free(states);
    return result;
}

static void dfs_generate_order(Graph* g, Stack* s, int* visited, int node) {
    visited[node] = 1;
    for (int i = 0; i < g->node_count; ++i) {
        if(graph_has_connection(g, node, i) && !visited[i]) {
            dfs_generate_order(g, s, visited, i);
        }
    }
    stack_push(s, node);
}

int graph_get_order(Graph* g, Stack** result) {
    if(!is_acyclic_graph(g)) return -1;
    
    *result = stack_create();
    int* visited = calloc(g->node_count, sizeof(int));
    if (visited == NULL)
        exit(0);
    
    for (int i = 0; i < g->node_count; ++i) {
        if(!visited[i]) dfs_generate_order(g, *result, visited, i);
    }
    
    free(visited);
    return 0;
}