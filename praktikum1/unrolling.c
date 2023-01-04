#include "size.h"

extern void matrix_mult(float a[SIZE][SIZE], float b[SIZE][SIZE], float res[SIZE][SIZE]) {
  for (int r = 0; r < SIZE; r++)
    for (int c = 0; c < SIZE; c++)
      for (int i = 0; i < SIZE; i += 4) {
        res[r][c] += a[r][i + 0] * b[i + 0][c];
        res[r][c] += a[r][i + 1] * b[i + 1][c];
        res[r][c] += a[r][i + 2] * b[i + 2][c];
        res[r][c] += a[r][i + 3] * b[i + 3][c];
      }
}
