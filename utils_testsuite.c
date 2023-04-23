#include <stdlib.h>
#include "miniunit.h"

int _dummy2() {
	mu_start();
	//
	mu_check(1 + 1 == 2);
	//
	mu_end();
}

int utils_tests() {	
	mu_run(_dummy2);
	return EXIT_SUCCESS;
}
