#include <stdlib.h>
#include <time.h>

#ifndef SIZE
#define SIZE 100
#endif /* !SIZE */

void init_random(float m[SIZE][SIZE]) {
  srand(time(0));
  for (int r = 0; r < SIZE; r++)
    for (int c = 0; c < SIZE; c++)
      m[r][c] = (float)rand();
}

void init_zero(float m[SIZE][SIZE]) {
  for (int r = 0; r < SIZE; r++)
    for (int c = 0; c < SIZE; c++)
      m[r][c] = 0;
}
