#include <cblas.h>
#include <stdio.h>
#include "size.h"

float a[SIZE][SIZE];
float b[SIZE][SIZE];
float res[SIZE][SIZE];
float cblas_res[SIZE][SIZE];

extern void init_random(float[SIZE][SIZE]);
extern void init_zero(float[SIZE][SIZE]);
extern void matrix_mult(float[SIZE][SIZE], float[SIZE][SIZE], float[SIZE][SIZE]);

int main() {
  init_random(a);
  init_random(b);
  init_zero(res);

  cblas_sgemm(CblasColMajor, CblasNoTrans, CblasNoTrans, SIZE, SIZE, SIZE, 1.0,
              &a[0][0], SIZE, &b[0][0], SIZE, 0.0, &cblas_res[0][0], SIZE);
  matrix_mult(a, b, res);

  for (int r = 0; r < SIZE; r++)
    for (int c = 0; c < SIZE; c++)
      if (cblas_res[r][c] != res[r][c]) {
        printf("Check failed for row(%d) column(%d): cblas(%f) != res(%f)\n", r, c, cblas_res[r][c], res[r][c]);
      }
}
