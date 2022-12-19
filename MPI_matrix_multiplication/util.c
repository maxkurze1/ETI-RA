#include <stdlib.h>
#include <time.h>
#include "size.h"

void init_random(float m[LEN]) {
    srand(time(0));
    for (int i = 0; i < LEN; i++)
            m[i] = (float)rand();
}

void init_zero(float m[LEN]) {
    for (int i = 0; i < LEN; i++)
            m[i] = 0;
}
