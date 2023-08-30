#include <stdlib.h>
#include <stdio.h> 
#include <stdint.h>
#include <stdbool.h>
// #include <omp.h>
#include "graphs.h"
#include "network.h"
#include "utils.h"

#define MIN_GRAPH_SIZE 2
#define MAX_GRAPH_SIZE 22
#define MEAN_SPARSITY 0.5
#define GRAPHS_PER_SIZE 1000
#define TRIALS_PER_GRAPH 400

#define END_TIME 100.0
#define STEPS 1000

#define PUMP_RATE 0.9
#define COUPLING_COEFFICIENT 0.1
#define NOISE 0.001

int main(int argc, char ** argv) {

  Graph* graph;
  Network* network;
  int max_cut_for_graph;
  
  for (int size = MIN_GRAPH_SIZE; size <= MAX_GRAPH_SIZE; size++) {
    int success = 0; 
    for (int _gn = 0; _gn < GRAPHS_PER_SIZE; _gn++) {
      graph = construct_graph(size);
      graph = random_mean_sparsity_graph(graph, MEAN_SPARSITY);

      // print_adjacency_matrix(*graph);
      
      max_cut_for_graph = max_cut(*graph);
      network = construct_network_from_graph(PUMP_RATE, COUPLING_COEFFICIENT, NOISE, graph, &kraymer_moyal, &euler_maruyama);

      int cut;
      for (int _tr = 0; _tr < TRIALS_PER_GRAPH; _tr++) {
        network_run(network, END_TIME, STEPS);
        network_get_partition_array(network);
        cut = evaluate_cut(*graph, network -> partition_array);
        success += cut == max_cut_for_graph;
      }

      destruct_graph(graph);
      destruct_network(network);
    }
    
    printf("%d %.4f\n", size, (float) success / (TRIALS_PER_GRAPH * GRAPHS_PER_SIZE));
  }
}
