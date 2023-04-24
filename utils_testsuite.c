#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "miniunit.h"
#include "utils.h"

int _descriptive_statistics() {
	mu_start();
	//
	int num_tests = 100000;
	double rand_nums[100000] = {0.0};
	double total = 0.0;

	for(int i = 0; i < num_tests; i++) {
		rand_nums[i] = rand_double();
		total += rand_nums[i];
	}
	double mean = total / num_tests;
	
	total = 0.0;
	for(int j = 0; j < num_tests; j++) {
		total += pow((rand_nums[j] - mean), 2.0);	
	}
	double variance = total / num_tests;
	
	printf("Mean: %.5f\n", mean);
	printf("Variance: %.5f\n", variance);
		
	//
	mu_end();
}

int _rand_double_time_1() {
	mu_start();
	//
	double time_2e7, time_1_ns;
	clock_t start, end;
	int num_rands = 20000000;
	int s_to_ns = 1000000000;

	start = clock();
	for(long i = 0; i < num_rands; i++) {
		rand_double();
	}
	end = clock();

	time_2e7 = (((double)(end - start)) / CLOCKS_PER_SEC);
	time_1_ns = (time_2e7 / num_rands) * s_to_ns;
	
	printf("Time(2E7): %3.5f s\n", time_2e7);
	printf("Time(1):   %2.5f ns\n", time_1_ns);
	//
	mu_end();
}

int utils_tests() {	
	mu_run(_descriptive_statistics);
	mu_run(_rand_double_time_1);
	return EXIT_SUCCESS;
}
