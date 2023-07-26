#include <stdlib.h>
#include <stdio.h> 
#include <stdint.h>
#include <stdbool.h>
#include <omp.h>
#include "graphs.h"
#include "utils.h"


int main(int argc, char ** argv) {

  Graph* graph;
  int max_cut_for_graph;
  for (int i = 0; i < 1; i++) {
    graph = construct_graph(24);
    graph = random_mean_sparsity_graph(graph, 0.95);
    print_adjacency_matrix(*graph);
    
    max_cut_for_graph = max_cut(*graph);
    graph -> edges[0][0] = max_cut_for_graph;
    printf("MAX-CUT: %s%d%s\n", KYEL, max_cut_for_graph, KWHT);

    destruct_graph(graph);
    printf("\n");
  }

  /*
#pragma omp parallel 
{
int size = omp_get_thread_num() + 2;
Graph graph = *construct_graph(size);

int i;
int j;
for (i = 0; i < size; i++) {
for (j = 0; j < size; j++) {
graph.edges[i][j] = (i != j);
}
}

printf("Size: %2d, Max cut: %4d\n", size, max_cut(graph));
}

return EXIT_SUCCESS;
*/
}
