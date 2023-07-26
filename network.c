#include "network.h"

Network* constructi_network_from_graph(int size, double coupling_strength, Graph graph) {
  // Create Network
  Network* network = malloc(sizeof(Network));
  if (network == NULL) return NULL;
  
  network -> size = size;
  
  // Allocate memory for amplitude state vector
  network -> amplitudes = malloc(sizeof(double) * size * 2);
  if (network -> amplitudes == NULL) {
    free(network);
    return NULL;
  }

  // Allocate memory for coupling matrix
  network -> couplings = malloc(sizeof(double*) * size);
  if (network -> couplings == NULL) {
    free(network -> amplitudes);
    free(network);
    return NULL;
  }
  
  // Allocate memory for coupling subarrays
  int i;
  for (i = 0; i < size; i++) {
    (network -> couplnigs)[i] = malloc(sizeof(double) * size);
  }

  // If any mallocs were unsuccessful, 
  // free all allocated memory and return NULL
  bool success = true;
  for (i = 0; i < size; i++) {
    if ((network -> couplings)[i] == NULL) success = false;
  }

  if (!success) {
    for (i = 0; i < size; i++) {
      // Test to make sure edge isn't NULL before freeing
      if ((network -> couplings)[i] != NULL) free((network -> couplings)[i]);
    }
    free(network -> couplings);
    free(network -> amplitudes);
    free(network);

    return NULL;
  }

  return network;
}

void destruct_network(Network* network) {
  int i;
  for (i = 0; i < network -> size; i++) {
    free((network -> couplings)[i]);
  }
  free(network -> couplings);
  free(network -> amplitudes);
  free(network);
}



