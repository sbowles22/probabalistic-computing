#include <stdlib.h>
#include "miniunit.h"
#include "graph_testsuite.h"

int _dummy() {
	mu_start();
	//
	mu_check(1 + 1 == 2);
	//
	mu_end();
}



int graph_tests() {	
	mu_run(_dummy);
	return EXIT_SUCCESS;
}
