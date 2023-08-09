//
//  utils.c
//
//  An assortment of utility functions unrelated to graphs or stochastic integration
//

#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "xoshiro256plus.c"

#define M_PI 3.14159265358979323846

// Both are 64 bit types
// required for floating point bit manipulation
typedef union v64_union {
  double f;
  uint64_t u;
} v64;

// stole from https://gist.github.com/mhcoma/3afe0f4a990e66366a9fa28d73b7e440
// For debugging
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

typedef union {
  uint64_t i;
  double d;
} double_uint64_union;

// rand_double
// Generates a random double value between 0 and 1
// Uniformly distrobuted
// See IEEE 754 standard for explanation of double type
double rand_double(void) {
  v64 random;
  random.u = 0;

  // uint64_t res = next();
  // Set mantissa 1.X_1 X_2 X_3 ... X_52 to random bits
  random.u |= next() >> 12;
  // Set exponent to 1023, which corresponds to 2^0
  random.u |= (uint64_t) 0x3ff << 52; 
  // Shift down by one to get random double between 0 and 1 
  random.f -= 1.0; 

  return random.f;
}

// 
// Using Box-Muller transform
// inspired by https://cse.usf.edu/~kchriste/tools/gennorm.c
//
// Since transform generates 2 intependent random numbers, 
// the additional is stored in a holding variable
//

static double _rand_normal_holding[3] = {NAN, NAN, NAN}; // holds extra double generated, should be struct but im lazy

double rand_norm(double mean, double std_dev) {
  double rand_normal;
  
  if (!isnan(_rand_normal_holding[0]) && _rand_normal_holding[1] == mean && _rand_normal_holding[2] == std_dev) {
    rand_normal = _rand_normal_holding[0];
    _rand_normal_holding[0] = NAN;
    return rand_normal;
  }
  
  double u;
  double r;

  do u = rand_double(); while (u == 0.0);
  r = sqrt(-2.0 * log(u));
  
  u = 2.0 * M_PI * rand_double(); // technically theta but saves memory

  rand_normal = r * cos(u) * std_dev + mean;
  _rand_normal_holding[0] = r * sin(u) * std_dev + mean;
  _rand_normal_holding[1] = mean;
  _rand_normal_holding[2] = std_dev;
  
  return rand_normal;
}
