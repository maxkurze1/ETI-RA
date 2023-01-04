#include "size.h"

extern void matrix_mult(float a[SIZE][SIZE], float b[SIZE][SIZE], float res[SIZE][SIZE]) {
  for (int r = 0; r < SIZE; r++)
    for (int i = 0; i < SIZE; i++)
      for (int c = 0; c < SIZE; c++)
        res[r][c] += a[r][i] * b[i][c];
}
