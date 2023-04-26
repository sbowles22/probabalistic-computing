//
//  Code for graphs
//
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "graphs.h"
#include "utils.h"

Graph* construct_graph(int size) {
  // Create Graph
  Graph* graph = malloc(sizeof(Graph));
  if (graph == NULL) return NULL;
  
  graph -> size = size;
  graph -> edges = malloc(sizeof(int*) * size);
  if (graph -> edges == NULL) {
    free(graph);
    return NULL;
  }
  
  // Allocate memory for subarrays
  int i;
  for (i = 0; i < size; i++) {
    (graph -> edges)[i] = malloc(sizeof(int) * size);
  }

  // If any mallocs were unsuccessful, 
  // free all allocated memory and return NULL
  bool success = true;
  for (i = 0; i < size; i++) {
    if ((graph -> edges)[i] == NULL) success = false;
  }

  if (!success) {
    for (i = 0; i < size; i++) {
      // Test to make sure edge isn't NULL before freeing
      if ((graph -> edges)[i] != NULL) free((graph -> edges)[i]);
    }
    free(graph -> edges);
    free(graph);

    return NULL;
  }

  return graph;
}

void print_adjacency_matrix(Graph graph) {
  int i;
  int j;
  for (i = 0; i < graph.size; i++) {
    for (j = 0; j < graph.size; j++) {
      printf("%s%d ", graph.edges[i][j] ? KWHT : KGRN, graph.edges[i][j]);
    }
    printf("%s\n", KWHT);
  }
}

Graph* random_mean_sparsity_graph(Graph* graph, double mean_sparsity) {
  int i;
  int j;
  bool edge;
  for (i = 0; i < graph -> size; i++) {
    graph -> edges[i][i] = 0;
    for (j = i + 1; j < graph -> size; j++) {
      edge = mean_sparsity < rand_double();
      graph -> edges[i][j] = edge;
      graph -> edges[j][i] = edge;
    }
  }
  return graph;
}


Graph random_cubic_graph(Graph graph, int size) {
  // TODO Write function
  return graph;
}

void destruct_graph(Graph* p_graph) {
  int i;
  for (i = 0; i < p_graph -> size; i++) {
    free((p_graph -> edges)[i]);
  }

  free(p_graph -> edges);
  free(p_graph);
}

// Max cut function and helper
int max_cut(Graph graph) {
  int* partition_array = calloc(graph.size, sizeof(int));
  if (partition_array == NULL) return -1;
  
  // Destination of max cut
  int max_value = 0;

  _max_cut(graph, 0, partition_array, &max_value);

  return max_value;
}

void _max_cut(Graph graph, int node, int* partition_array, int* p_max_value) {
  // Base case
  if (node == graph.size) {
    int cut_value;
    cut_value = evaluate_cut(graph, partition_array);
    
    if (cut_value > *p_max_value) *p_max_value = cut_value;
    return;
  }
  
  // Check both possible partitions of the next node
  partition_array[node] = 1;
  _max_cut(graph, node + 1, partition_array, p_max_value);
  partition_array[node] = -1;
  _max_cut(graph, node + 1, partition_array, p_max_value);
}

int evaluate_cut(Graph graph, int* partition_array) {
  int i;
  int j;
  int count = 0;
  for (i = 0; i < graph.size; i++) {

    // Only check first partition 1 to prevent double counting
    if (partition_array[i] == 1) {
      for (j = 0; j < graph.size; j++) {
        // Add edge weight if edge between i and j 
        // and they are in different partitions
        count += graph.edges[i][j] * (partition_array[i] != partition_array[j]);
      }
    }
  }

  return count;
}

