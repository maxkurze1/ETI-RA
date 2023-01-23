#include <stdio.h>
#include "../size.h"

double a[SIZE][SIZE];
double b[SIZE][SIZE];
double res[SIZE][SIZE];
double correct[SIZE][SIZE];

extern void init_random(double[SIZE][SIZE]);
extern void init_zero(double[SIZE][SIZE]);
extern void matrix_mult(double[SIZE][SIZE], double[SIZE][SIZE], double[SIZE][SIZE]);

int main() {
  init_random(a);
  init_random(b);
  init_zero(res);

  for (int r = 0; r < SIZE; r++)
    for (int c = 0; c < SIZE; c++)
      for (int i = 0; i < SIZE; i++)
        correct[r][c] += a[r][i] * b[i][c];

  matrix_mult(a, b, res);

  int count = 0;
  for (int r = 0; r < SIZE; r++)
    for (int c = 0; c < SIZE; c++)
      if (correct[r][c] != res[r][c]) {
        printf("Check failed for row(%d) column(%d):\ncorrect(%lf) !=\nres    (%lf)\n", r, c, correct[r][c], res[r][c]);
        if (++count >= 5) return 1;
      }
  if (count == 0) printf("No failes\n");
  return count != 0;
}
