#include "../size.h"

extern void matrix_mult(double a[SIZE][SIZE], double b[SIZE][SIZE], double res[SIZE][SIZE]) {
  #pragma omp target teams map(to: a[:SIZE][:SIZE], b[:SIZE][:SIZE]) map(from: res[:SIZE][:SIZE])
  #pragma omp distribute parallel for collapse(2)
  for (int r = 0; r < SIZE; r++){
    for (int c = 0; c < SIZE; c++){
      for (int i = 0; i < SIZE; i++)
        res[r][c] += a[r][i] * b[i][c];
    }
  }
}
