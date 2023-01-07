#include "default.h"
#include <cstdlib>

double a[SIZE][SIZE];
double b[SIZE][SIZE];
double res[SIZE][SIZE];

extern "C" void init_random(double[SIZE][SIZE]);
extern "C" void init_zero(double[SIZE][SIZE]);

int main(void) {
  init_random(a);
  init_random(b);
  init_zero(res);
  
  cuda_matrix_mult(a,b,res);

  return 0;
}
