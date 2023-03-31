#include <stdlib.h>
#include <stdio.h> 
#include <stdbool.h>
#include <omp.h>
#include "graphs.h"

int main(int argc, char ** argv) {

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
}
