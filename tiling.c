#include "size.h"

extern void matrix_mult(float a[SIZE][SIZE], float b[SIZE][SIZE], float res[SIZE][SIZE]) {
  // looping through all tiles
  for (int rt = 0; rt < SIZE/TILE_SIZE; rt++)
    for (int ct = 0; ct < SIZE/TILE_SIZE; ct++)
      for (int it = 0; it < SIZE/TILE_SIZE; it++)
        // small matrix multiplication to calculate tile
        for (int r = 0; r < TILE_SIZE; r++)
          for (int c = 0; c < TILE_SIZE; c++)
            for (int i = 0; i < TILE_SIZE; i++)
              res[rt*TILE_SIZE + r][ct*TILE_SIZE + c] += a[rt*TILE_SIZE + r][it*TILE_SIZE + i] * b[it*TILE_SIZE + i][ct*TILE_SIZE + c];
}
