#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "network.h"

Network* construct_network_from_graph(double p, double coupling_strength, Graph graph, Gradient gradient, Solver solver) {
  // Create Network
  Network* network = malloc(sizeof(Network));
  if (network == NULL) return NULL;
  
  int size = graph.size;
  network -> size = size;
  network -> p = p;
  
  // Allocate memory for amplitude state vector
  network -> amplitudes = malloc(sizeof(double) * size * 2);
  if (network -> amplitudes == NULL) {
    free(network);
    return NULL;
  }

  // Allocate memory for amplitude state vector
  network -> dadt = malloc(sizeof(double) * size * 2);
  if (network -> dadt == NULL) {
    free(network -> amplitudes);
    free(network);
    return NULL;
  }

  // Allocate memory for coupling matrix
  network -> couplings = malloc(sizeof(double*) * size);
  if (network -> couplings == NULL) {
    free(network -> amplitudes);
    free(network -> dadt);
    free(network);
    return NULL;
  }
  
  // Allocate memory for coupling subarrays
  int i;
  for (i = 0; i < size; i++) {
    (network -> couplings)[i] = malloc(sizeof(double) * size);
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
      if ((network -> couplings)[i] != NULL) {
        free((network -> couplings)[i]);
      }
    }
    free(network -> couplings);
    free(network -> amplitudes);
    free(network -> dadt);
    free(network);

    return NULL;
  }

  network -> solver = solver;
  network -> gradient = gradient;

  // Initialize network amplitudes to 0
  for (i = 0; i < size; i++) {
    (network -> amplitudes)[i] = 0.0;
  }
  printf("%lf\n", (network -> amplitudes)[0]);

  return network;
}

void destruct_network(Network* network) {
  int i;
  for (i = 0; i < network -> size; i++) {
    free((network -> couplings)[i]);
  }
  free(network -> couplings);
  free(network -> amplitudes);
  free(network -> dadt);
  free(network);
}

// DOI: 10.1103/PhysRevA.88.063853
void kraymer_moyal (Network* network) {
  double* c = &((network -> amplitudes)[0]);
  double* s = &((network -> amplitudes)[network -> size]);
  double* dcdt = &((network -> dadt)[0]);
  double* dsdt = &((network -> dadt)[network -> size]);
  double p = network -> p;
  
  memset(dcdt, 0.0, (network -> size) * sizeof(double));
  memset(dsdt, 0.0, (network -> size) * sizeof(double));

  int i;
  int j;
  for (i = 0; i < network -> size; i++) {
    dcdt[i] += (-1 + p - (c[i]*c[i] + s[i]*s[i])) * c[i];
    for (j = 0; j < network -> size; j++) {
      dcdt[i] += (network -> couplings)[i][j] * c[j];
    }
  }

  for (i = 0; i < network -> size; i++) {
    dsdt[i] += (-1 - p - (c[i]*c[i] + s[i]*s[i])) * s[i];
    for (j = 0; j < network -> size; j++) {
      dsdt[i] += (network -> couplings)[i][j] * s[j];
    }
  }
}
