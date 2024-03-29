#ifndef __GRAPHS_H__
#define __GRAPHS_H__
#include <stdbool.h>

// Graph constructed by adjacency matrix
typedef struct {
  int size;
  unsigned char** edges; // n by n edge array
} Graph;

// Constructor and destructor
Graph* construct_graph(int size);
void destruct_graph(Graph* graph);

// Print graph
void print_adjacency_matrix(Graph graph); 

// Random graph generators
Graph* random_mean_sparsity_graph(Graph* graph, double mean_sparsity);
Graph random_cubic_graph(Graph graph, int size);

// Max cut function and helper
int max_cut(Graph graph);
void _max_cut(Graph graph, int node, int* partition_array, int* p_max_value); 
int evaluate_cut(Graph graph, int* partition_array);

#endif
