#ifndef __UTILS_H__
#define __UTILS_H__

//
// utils.h
//

// Colors for fun printing!
#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

// Random uniform number generators
double rand_double(void);

// Random normal number generators
double rand_norm(double mean, double std_dev);

#endif
