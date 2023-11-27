// 
// network.h
//
// includes functions about opo networks
//

#include "graphs.h"
#define LOG_FILENAME "log.log"

typedef struct _Network {
  int size;            // size of network
  double sim_time;     // simulation time

  double p;            // normalized_pump_rate
  double noise;        // normalized noise

  double** couplings;  // size by size array of couplings between OPOs
  double* c;           // size array of state of amplitudes, organized [c1,...,cn]
  double* s;           // size array of state of amplitudes, organized [s1,...sn]
  double* dcdt;        // size array of state of amplitude differentials, organized [dc1,...,dcn]
  double* dsdt;        // size array of state of amplitude differentials, organized [ds1,...,dsn]

  int* partition_array; // Array for cut

  void (*solver) (struct _Network* network, double h); // SDE Integrator
  void (*gradient) (struct _Network* network);         // Generates the current dA/dt using kramer moyal 
} Network;

typedef void (*Solver) (Network* network, double h);
typedef void (*Gradient) (Network* network);

// Constructors and destructors
Network* construct_network_from_graph(double p, double coupling_strength, double noise, Graph* graph, Gradient gradient, Solver solver);
void destruct_network(Network* network);

// Functions for preparing Network for simulation
void reset_network_state(Network* network);

void kraymer_moyal (Network* network);
void euler_maruyama(Network* network, double h);

void network_run(Network* network, Graph* graph, double time_final, int steps);
void network_get_partition_array(Network* network);
