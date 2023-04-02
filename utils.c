//
//  utils.c
//
//  An assortment of utility functions unrelated to graphs or stochastic integration
//

#include <stdint.h>
#include "xoshiro256plus.c"

typedef union {
  uint64_t i;
  double d;
} double_uint64_union;

// rand_double
// Generates a random double value
double rand_double(void) {
  double_uint64_union random;
  random.i = 0;
  
  uint64_t res = next();
  uint64_t sign = (res >> 63) << 63;
  res ^= sign;

  random.i |= next() >> 12;
  random.i |= (uint64_t) 0x3ff << 52;
  random.i |= sign; 

  return random.d;
}

