// 
// network.h
//
// includes functions about opo networks
//

#include "graph.h"

typedef struct {
  int size; // size of network
  double** couplings; // size by size array of couplings between OPOs
  double sim_time; // simulation time
  double* amplitudes; // 2*size array of state of amplitudes, organized [c1,...,cn,s1,...sn]
} Network;

// Constructors and destructors
Network* construct_network_from_graph(int size, double coupling_strength, Graph graph);
void destruct_network(Network* network);

// Functions for preparing Network for simulation
void reset_network_state(Network* network);

