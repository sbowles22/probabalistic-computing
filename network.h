// 
// network.h
//
// includes functions about opo networks
//

#include "graphs.h"

typedef struct _Network {
  int size;            // size of network
  double p;            // normalized_pump_rate
  double** couplings;  // size by size array of couplings between OPOs
  double sim_time;     // simulation time
  double* amplitudes;  // 2*size array of state of amplitudes, organized [c1,...,cn,s1,...sn]
  double* dadt;        // 2*size array of state of amplitude differentials, organized [c1,...,cn,s1,...sn]

  void (*solver) (struct _Network* network, double h); // SDE Integrator
  void (*gradient) (struct _Network* network);         // Generates the current dA/dt using kramer moyal 
} Network;

typedef void (*Solver) (Network* network, double h);
typedef void (*Gradient) (Network* network);

// Constructors and destructors
Network* construct_network_from_graph(double p, double coupling_strength, Graph graph, Gradient gradient, Solver solver);
void destruct_network(Network* network);

// Functions for preparing Network for simulation
void reset_network_state(Network* network);

void kraymer_moyal (Network* network);
