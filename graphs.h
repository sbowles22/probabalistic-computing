#ifndef __GRAPHS_H__
#define __GRAPHS_H__
#include <stdbool.h>

typedef struct {
  int size;
  int** edges; // n by n edge array
} Graph;

// Constructor and destructor
Graph* construct_graph(int size);
void destruct_graph(Graph* graph);

// Turn graph into random cubic graph
Graph random_cubic_graph(Graph graph, int size);

// Max cut function and helper
int max_cut(Graph graph);
void _max_cut(Graph graph, int node, int* partition_array, int* p_max_value); 
int evaluate_cut(Graph graph, int* partition_array);

#endif
