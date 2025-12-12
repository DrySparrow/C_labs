#ifndef graph_h
#define graph_h

#include "stack.h"

typedef struct Graph Graph;

Graph* graph_init(int const nodes);
void graph_destroy(Graph* g);
int graph_add_connection(Graph* g, int src, int dst);
int graph_has_connection(Graph* g, int const src, int dst);
int is_acyclic_graph(Graph* g);
int graph_get_order(Graph* g, Stack** result);

#endif