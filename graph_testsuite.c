#include <stdlib.h>
#include "miniunit.h"

#include "graph_testsuite.h"
#include "graphs.h"

int _max_cut_2_nodes() {
	mu_start();
	//
	Graph* graph;
	int m_c;
	int size = 2;
	
	graph = construct_graph(size);
	
	for(int i = 0; i < size; i++) {
		for(int j = 0; j < size; j++) {
			(graph -> edges)[i][j] = (i != j);
		}
	}
	
	m_c = max_cut(*graph);
	
	mu_end();
}



int graph_tests() {	
	mu_run(_max_cut_2_nodes);
	return EXIT_SUCCESS;
}
