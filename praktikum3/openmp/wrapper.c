#include <time.h>
#include <stdio.h>
#include "../size.h"

extern void init_random(double[SIZE][SIZE]);
extern void init_zero(double[SIZE][SIZE]);
extern void matrix_mult(double[SIZE][SIZE], double[SIZE][SIZE], double[SIZE][SIZE]);

double a[SIZE][SIZE];
double b[SIZE][SIZE];
double res[SIZE][SIZE];

int main(int argc, char *argv[]) {
  int launch_count = 1;
  if (argc > 1)
    sscanf(argv[1], "%d", &launch_count);

  for (; launch_count > 0; launch_count--) {
    init_random(a);
    init_random(b);
    init_zero(res);

    matrix_mult(a, b, res);
  }
}
