#include "size.h"
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#define MIN_EXP -500L // default -1023
#define MAX_EXP 500L // default 1024
// 2^a * 2^b = 2^(a+b) therefore exp should stay smaller than (default/2) to avoid Inf

double random_double() {
  // 1bit sign
  long sign = (rand() & 0x1L);
  // exponent has size of 11bit
  // avoid NaN or Inf => they destroy the check of the arithmetic precision
  // using different exponent bounds to reduce the occurrency of NaN and Inf inside matrix calculations
  long exp = (rand() % (MAX_EXP - MIN_EXP)) + 1023L + MIN_EXP;
  // mantissa generated in blocks of 15 because rand() can only guaranty to generate at least 15 random bits (RAND_MAX >= 2^15)
  long mant = ((rand() & 0x7FL) << 45) | ((rand() & 0x7FFFL) << 30) | ((rand() & 0x7FFFL) << 15) | ((rand() & 0x7FFFL) << 0x0);

  long rand = (sign << 63) | (exp << 52) | (mant);
  // reinterpret to keep binary layout
  return *(double *)&rand;
}

bool once = true;
void init_random(double m[SIZE][SIZE]) {
  // only init with time() once because time only has a precision of a second
  // else calling srand() multiple time a second could lead to the same seeds
  if (once) { srand(time(0)); once = false; }
  for (int r = 0; r < SIZE; r++)
    for (int c = 0; c < SIZE; c++)
      m[r][c] = random_double();
}

void init_zero(double m[SIZE][SIZE]) {
  for (int r = 0; r < SIZE; r++)
    for (int c = 0; c < SIZE; c++)
      m[r][c] = 0;
}
