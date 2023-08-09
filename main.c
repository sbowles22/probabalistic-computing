#include <stdlib.h>
#include <stdio.h> 
#include <stdint.h>
#include <stdbool.h>
// #include <omp.h>
#include "graphs.h"
#include "network.h"
#include "utils.h"


int main(int argc, char ** argv) {

  Graph* graph;
  Network* network;
  int max_cut_for_graph;
  for (int i = 0; i < 1; i++) {
    graph = construct_graph(2);
    graph = random_mean_sparsity_graph(graph, 0.0);

    // graph = construct_graph(40);
    // graph = random_mean_sparsity_graph(graph, 0.7);
    // print_adjacency_matrix(*graph);
    
    max_cut_for_graph = max_cut(*graph);
    printf("MAX-CUT: %s%d%s\n", KYEL, max_cut_for_graph, KWHT);

    network = construct_network_from_graph(1.1, 0.1, 0.001, *graph, &kraymer_moyal, &euler_maruyama);

    FILE* u = fopen("u.txt", "w");
    FILE* v = fopen("v.txt", "w");
    for (double x = -2.0; x < 2.1; x += 0.2) {      
      for (double y = -2.0; y < 2.1; y += 0.2) {
        (network -> c)[0] = x;
        (network -> c)[1] = y;
        (network -> gradient)(network);
        fprintf(u, "%lf ", (network -> dcdt)[0]);
        fprintf(v, "%lf ", (network -> dcdt)[1]);
      }
      fprintf(u, "\n");
      fprintf(v, "\n");
    }

    fclose(u);
    fclose(v);

    // int cut;
    // int success = 0; 
    // for (int j = 0; j < 1000; j++) {
    //   // (network -> gradient)(network);
    //   (network -> solver)(network, 0.01);
    //   network_run(network, 1.0, 10000);
    //   network_get_partition_array(network);
    //   cut = evaluate_cut(*graph, network -> partition_array);
    //   // printf("%+d %+d %d\n", (network -> partition_array)[0], (network -> partition_array)[1], cut);
    //   printf("%lf %lf\n", (network -> c)[0], (network -> c)[1]);
    //   success += cut == max_cut_for_graph;
    // }
    // printf("Accuracy: %.2f%%\n", (float) success);
    
    // for (int j = 0; j < (network -> size); j++) {
    //   printf("%lf", (network -> c)[j]);
    //   printf(" %lf\n",  (network -> dcdt)[j]);
    // }

    destruct_graph(graph);
    destruct_network(network);
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
