#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#ifndef __MINIUNIT_H__
#define __MINIUNIT_H__

#define MU_SUCCESS 0

#define mu_start() int __mu_first_failure_line_number_or_0 = MU_SUCCESS

#define mu_end() return __mu_first_failure_line_number_or_0

#define mu_run(function) 																	\
	do{  																					\
		int funcOutput = function(); 														\
		if(funcOutput == 0) { 																\
			printf("\x1b[32mTest passed: %s\x1b[0m\n", #function); 							\
		} 																					\
		else { 																				\
			printf("\x1b[31mTest failed: %s at line %d\x1b[0m\n", #function, funcOutput); 	\
		} 																					\
	} while(false)
	
#define mu_check(condition) 																			\
	do{  																								\
		if((condition ? true : false) == false && __mu_first_failure_line_number_or_0 == MU_SUCCESS) { 	\
			__mu_first_failure_line_number_or_0 = __LINE__; 											\
		} 																								\
	} while(false)

#define mu_check_strings_equal(s1, s2)	mu_check(strcmp(s1, s2) == 0)

#endif /* end of include guard: __MINIUNIT_H__ */

