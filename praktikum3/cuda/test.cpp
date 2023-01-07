#include "default.h"
#include <cstdio>
#include <cblas.h>

double a[SIZE][SIZE];// = {{1,2,3,4},{5,6,7,8},{9,10,11,12},{13,14,15,16}};
double b[SIZE][SIZE];// = {{1,0,3,4},{5,6,7,8},{9,10,11,12},{13,14,15,16}};
double res[SIZE][SIZE];
double correct[SIZE][SIZE];

extern "C" void init_random(double[SIZE][SIZE]);
extern "C" void init_zero(double[SIZE][SIZE]);

void print_binary(long value, int count){
  for (int i = count-1; i >= 0; i--)
    printf("%s", (value >> i) & 1 ? "1" : "0");
}

int main(void) {
  init_random(a);
  init_random(b);
  init_zero(res);

  cuda_matrix_mult(a, b, res);

  // cblas_dgemm(CblasColMajor, CblasNoTrans, CblasNoTrans, SIZE, SIZE, SIZE, 1.0, &b[0][0], SIZE, &a[0][0], SIZE, 0.0, &correct[0][0], SIZE);

  for (int r = 0; r < SIZE; r++)
    for (int c = 0; c < SIZE; c++)
      for (int i = 0; i < SIZE; i++)
        correct[r][c] += a[r][i] * b[i][c];

  int count = 0;
  for (int r = 0; r < SIZE; r++)
    for (int c = 0; c < SIZE; c++) {
      // long error = (*(long*)&correct[r][c] - *(long*)&res[r][c]); 
      if (correct[r][c] != res[r][c]) {
        printf("Check failed for row(%d) column(%d):\nc: ", r, c);
        print_binary(*(long*)&correct[r][c], 64);
        printf("\nr: ");
        print_binary(*(long*)&res[r][c], 64);
        // for (int i = 0; i < SIZE; i ++)
        //   printf("%lf * %lf%s", a[r][i], b[i][c], i < SIZE-1 ? " +" : "\n");
        // if (++count >= 1) return 1;
      }
    }
  if (count == 0) printf("No failes\n");
  return count != 0;
}
