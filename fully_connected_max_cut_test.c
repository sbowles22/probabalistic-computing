// Temporary file that contains code for testing max_cut function

#include <stdlib.h>
#include <stdio.h> 
#include <stdbool.h>
#include <omp.h>
#include "graphs.h"

int main(int argc, char ** argv) {
  Graph graph = *construct_graph(5);

  int i;
  int j;
  for (i = 0; i < 5; i++) {
    for (j = 0; j < 5; j++) {
      graph.edges[i][j] = (i != j);
    }
  }
  
  printf("%d\n", max_cut(graph));
  // #pragma omp parallel 
  // {
  //   printf("Hello from process: %d\n", omp_get_thread_num());
  // }

  return EXIT_SUCCESS;
}
