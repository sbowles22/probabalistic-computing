#include <stdlib.h>
#include "graphs.h"
#include "utils.h"
#include "miniunit.h"

#include "graph_testsuite.h"
#include "utils_testsuite.h"

static int _test_testsuite_compiles();

static int _test_testsuite_compiles() {
	mu_start();
	//

	mu_check(1 + 1 == 2);

	//
	mu_end();
}


int main(int argc, char** argv) {
	printf("\nTestsuite Compiles:\n");
	mu_run(_test_testsuite_compiles);
	
	printf("\nGraph Tests:\n");
	graph_tests();
	
	printf("\nTime Tests:\n");
	utils_tests();
	
	return EXIT_SUCCESS;
}
