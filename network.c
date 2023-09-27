#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <math.h>
#include <omp.h>
#include "network.h"
#include "utils.h"

Network* construct_network_from_graph(double p, double coupling_strength, double noise, Graph* graph, Gradient gradient, Solver solver) {
  // Create Network
  Network* network = malloc(sizeof(Network));
  if (network == NULL) return NULL;
  
  int i;
  int j;
  int size = graph -> size;
  network -> size = size;
  network -> p = p;
  network -> noise = noise;
  
  // Allocate memory for amplitude state vector
  network -> c = malloc(sizeof(double) * size);
  if (network -> c == NULL) {
    free(network);
    return NULL;
  }

  network -> s = malloc(sizeof(double) * size);
  if (network -> s == NULL) {
    free(network);
    free(network -> c);
    return NULL;
  }

  // Allocate memory for amplitude state vector
  network -> dcdt = malloc(sizeof(double) * size);
  if (network -> dcdt == NULL) {
    free(network -> c);
    free(network -> s);
    free(network);
    return NULL;
  }

  // Allocate memory for amplitude state vector
  network -> dsdt = malloc(sizeof(double) * size);
  if (network -> dsdt == NULL) {
    free(network -> c);
    free(network -> s);
    free(network -> dcdt);
    free(network);
    return NULL;
  }

  // Allocate memory for partition array
  network -> partition_array = malloc(sizeof(int) * size);
  if (network -> partition_array == NULL) {
    free(network -> c);
    free(network -> s);
    free(network -> dcdt);
    free(network -> dsdt);
    free(network);
    return NULL;
  }

  // Allocate memory for coupling matrix
  network -> couplings = malloc(sizeof(double*) * size);
  if (network -> couplings == NULL) {
    free(network -> c);
    free(network -> s);
    free(network -> dcdt);
    free(network -> dsdt);
    free(network -> partition_array);
    free(network);
    return NULL;
  }
  
  // Allocate memory for coupling subarrays
  for (i = 0; i < size; i++) {
    (network -> couplings)[i] = malloc(sizeof(double) * size);
  }

  // If any mallocs were unsuccessful, 
  // free all allocated memory and return NULL
  bool success = true;
  for (i = 0; i < size; i++) {
    if ((network -> couplings)[i] == NULL) {
      success = false;
    }
  }

  if (!success) {
    for (i = 0; i < size; i++) {
      // Test to make sure edge isn't NULL before freeing
      if ((network -> couplings)[i] != NULL) {
        free((network -> couplings)[i]);
      }
    }
    free(network -> couplings);
    free(network -> c);
    free(network -> s);
    free(network -> dcdt);
    free(network -> dsdt);
    free(network -> partition_array);
    free(network);

    return NULL;
  }

  network -> solver = solver;
  network -> gradient = gradient;

  // Initialize network amplitudes to 0
  for (i = 0; i < size; i++) {
    (network -> c)[i] = 0.0;
    (network -> s)[i] = 0.0;
  }
  // printf("%lf\n", (network -> amplitudes)[0]);

  for (i = 0; i < size; i++) {
    for (j = 0; j < size; j++) {
      int edge = (graph -> edges)[i][j];
      (network -> couplings)[i][j] = -coupling_strength * edge;
    }
  }

  return network;
}

void destruct_network(Network* network) {
  int i;
  for (i = 0; i < network -> size; i++) {
    free((network -> couplings)[i]);
  }
  free(network -> couplings);
  free(network -> c);
  free(network -> s);
  free(network -> dcdt);
  free(network -> dsdt);
  free(network -> partition_array);
  free(network);
}

// DOI: 10.1103/PhysRevA.88.063853
void kraymer_moyal (Network* network) {
  double* c = network -> c;
  double* s = network -> s;
  double* dcdt = network -> dcdt;
  double* dsdt = network -> dsdt;
  double p = network -> p;
  
  int i;
  // int j;

  for (i = 0; i < network -> size; i++) {
    dcdt[i] = 0.0;
    dsdt[i] = 0.0;
  }

  // memset(dcdt, 0.0, (network -> size) * sizeof(double));
  // memset(dsdt, 0.0, (network -> size) * sizeof(double));

  
  #pragma omp parallel for schedule(dynamic, 50)
  for (i = 0; i < network -> size; i++) {
    dcdt[i] += (-1 + p - (c[i]*c[i] + s[i]*s[i])) * c[i];
    for (int j = 0; j < network -> size; j++) {
      dcdt[i] += (network -> couplings)[i][j] * c[j];
    }
  }

  #pragma omp parallel for schedule(dynamic, 50)
  for (i = 0; i < network -> size; i++) {
    dsdt[i] += (-1 - p - (c[i]*c[i] + s[i]*s[i])) * s[i];
    // printf("%d: %lf\n", T++, dsdt[i]);
    for (int j = 0; j < network -> size; j++) {
      dsdt[i] += (network -> couplings)[i][j] * s[j];
      // printf("%d: %lf %lf %lf\n", T++, dsdt[i], (network -> couplings)[i][j], s[j]);
    }
  }
}

void euler_maruyama(Network* network, double h) {
  double* c = network -> c;
  double* s = network -> s;
  double* dcdt = network -> dcdt;
  double* dsdt = network -> dsdt;

  double sqrt_h = sqrt(h); 

  (network -> gradient)(network);

  // #pragma omp parallel for schedule(dynamic, 50)
  for (int i = 0; i < network -> size; i++) {
    // printf("C[%d] = %lf :: DCDT[%d] = %lf\n", i, c[i], i, dcdt[i]);
    c[i] += dcdt[i] * h + (network -> noise) * rand_norm(0, sqrt_h);
  }

  // #pragma omp parallel for schedule(dynamic, 50)
  for (int i = 0; i < network -> size; i++) {
    s[i] += dsdt[i] * h + (network -> noise) * rand_norm(0, sqrt_h);
  }
}

void network_run(Network* network, Graph* graph, double time_final, int steps) {
  if (steps == 0) {
    perror("Run cannot have ZERO steps");
    exit(1);
  }

  for (int i = 0; i < network -> size; i++) {
    (network -> c)[i] = 0.0;
    (network -> s)[i] = 0.0;
  }
  
  double h = time_final / (double) steps;
  int time_start = time(NULL);
  for (int i = 0; i < steps; i++) {
    if (i % (steps / 100) == 1) {
      int time_past = time(NULL) - time_start;
      int time_predicted = (float) steps / i * (float) time_past;
      
      network_get_partition_array(network);
      int cut = evaluate_cut(*graph, network -> partition_array);
      
      printf("%03d%% | ELAPSED: %03d:%02d:%02d, ETA: %03d:%02d:%02d | %10d\n", 
        (int) ((float) i / steps * 100),
        time_past / 3600, time_past / 60 % 60, time_past % 60,
        time_predicted / 3600, time_predicted / 60 % 60, time_predicted % 60,
        cut
      );
    }
    (network -> solver)(network, h);
  }
}

void network_get_partition_array(Network* network) {
  int i;
  for (i = 0; i < network -> size; i++) {
    if ((network -> c)[i] > 0) {
      (network -> partition_array)[i] = 1;
    }
    else {
      (network -> partition_array)[i] = -1;
    }
  }
}
