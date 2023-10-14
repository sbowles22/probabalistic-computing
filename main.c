#include <stdlib.h>
#include <stdio.h> 
#include <stdint.h>
#include <stdbool.h>
#include <omp.h>
#include <time.h>
#include "graphs.h"
#include "network.h"
#include "utils.h"

#define GRAPH_SIZE 10000
#define MEAN_SPARSITY_MIN 0.01
#define MEAN_SPARSITY_MAX 0.99
#define MEAN_SPARSITY_INC 0.01
#define GRAPHS_PER_SPARSITY 1
#define TRIALS_PER_GRAPH 40

#define END_TIME 10.0
#define STEPS 10000

#define PUMP_RATE 0.9
#define COUPLING_COEFFICIENT 0.1
#define NOISE 0.001

int main(int argc, char ** argv) {
  omp_set_num_threads(1);

  Graph* graph;
  Network* network;
  
  graph = construct_graph(GRAPH_SIZE);
  graph = random_mean_sparsity_graph(graph, 0.5);
    
  // max_cut_for_graph = max_cut(*graph);
  network = construct_network_from_graph(PUMP_RATE, COUPLING_COEFFICIENT, NOISE, graph, &kraymer_moyal, &euler_maruyama);

  network_run(network, graph, END_TIME, STEPS);
  network_get_partition_array(network);
  int cut = evaluate_cut(*graph, network -> partition_array);
  printf("%d\n", cut);

  destruct_graph(graph);
  destruct_network(network);
}

