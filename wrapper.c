#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifndef SIZE
#define SIZE 100
#endif /* !SIZE */

float a[SIZE][SIZE];
float b[SIZE][SIZE];
float res[SIZE][SIZE];

void init(float[SIZE][SIZE]);
void zero(float[SIZE][SIZE]);
extern void matrix_mult(float[SIZE][SIZE], float[SIZE][SIZE],
                        float[SIZE][SIZE]);

int executions = 0;
clock_t total_clock = 0;

int main() {
  while ((double)total_clock / CLOCKS_PER_SEC < 1.0) {

    init(a);
    init(b);
    zero(res);

    clock_t start = clock();
    // ====================================

    matrix_mult(a, b, res);
    for (int r = 0; r < SIZE; r++)
      for (int c = 0; c < SIZE; c++)
        for (int i = 0; i < SIZE; i++)
          res[r][c] += a[r][i] * b[i][c];

    // ====================================
    clock_t end = clock();

    // printf("%d: %f\n", executions, (double)(end - start) / CLOCKS_PER_SEC);

    executions++;
    total_clock += end - start;
  }
  printf("%f\n", (double)total_clock / (CLOCKS_PER_SEC * executions));
}

void init(float m[SIZE][SIZE]) {
  srand(time(0));
  for (int r = 0; r < SIZE; r++)
    for (int c = 0; c < SIZE; c++)
      m[r][c] = (float)rand();
}

void zero(float m[SIZE][SIZE]) {
  for (int r = 0; r < SIZE; r++)
    for (int c = 0; c < SIZE; c++)
      m[r][c] = 0;
}
