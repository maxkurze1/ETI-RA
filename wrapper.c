#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "size.h"

float a[SIZE][SIZE];
float b[SIZE][SIZE];
float res[SIZE][SIZE];

extern void init_random(float[SIZE][SIZE]);
extern void init_zero(float[SIZE][SIZE]);
extern void matrix_mult(float[SIZE][SIZE], float[SIZE][SIZE], float[SIZE][SIZE]);

int executions = 0;
clock_t total_clock = 0;

int main() {
  while ((double)total_clock / CLOCKS_PER_SEC < 1.0) {
    init_random(a);
    init_random(b);
    init_zero(res);

    // do multiplication
    clock_t start = clock();
    matrix_mult(a, b, res);
    clock_t end = clock();

    // printf("%d: %f\n", executions, (double)(end - start) / CLOCKS_PER_SEC);

    executions++;
    total_clock += end - start;
  }
  printf("%f\n", (double)total_clock / (CLOCKS_PER_SEC * executions));
}
