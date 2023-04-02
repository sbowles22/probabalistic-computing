#include <stdlib.h>
#include <stdio.h> 
#include <stdint.h>
#include <stdbool.h>
#include <omp.h>
#include "graphs.h"
#include "utils.h"


typedef union v64_union {
  double f;
  uint64_t u;
} v64;

void print_double_bits (double d) {
  v64 v; v.f = d;
  uint64_t mask = 1ULL << 63;
  int count = 63;
  do {
    if (mask == 0x4000000000000000 || mask == 0x8000000000000) putchar(' ');
    putchar(v.u & mask ? '1' : '0');
    count--;
  } while (mask >>= 1);
}

int main(int argc, char ** argv) {
  
  double res;
  for (int i = 0; i < 10; i++) {
    res = rand_double();
    print_double_bits(res);
    // print_double_bits(res);
    printf("\n%f\n", res);
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
