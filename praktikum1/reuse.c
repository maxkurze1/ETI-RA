#include "size.h"

extern void matrix_mult(float a[SIZE][SIZE], float b[SIZE][SIZE], float res[SIZE][SIZE]) {
  for (int r = 0; r < SIZE; r++)
    for (int c = 0; c < SIZE; c++){
      float sum = 0;
      for (int i = 0; i < SIZE; i++)
        sum += a[r][i] * b[i][c];
      res[r][c] = sum;
    }
}
