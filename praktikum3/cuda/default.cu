#include "default.h"
#include <cstdio>

__global__ void matrix_mult(double *a, double *b, double *res) {
  int r = blockIdx.x * blockDim.x + threadIdx.x;
  int c = blockIdx.y * blockDim.y + threadIdx.y;

  for (int i = 0; i < SIZE; i++)
    res[r * SIZE + c] += a[r * SIZE + i] * b[i * SIZE + c];
}

void cuda_matrix_mult(double a[SIZE][SIZE], double b[SIZE][SIZE], double res[SIZE][SIZE]) {
  double *gpu_a;
  double *gpu_b;
  double *gpu_res;

  constexpr size_t byte_size = SIZE * SIZE * sizeof(double);

  cudaMalloc(&gpu_a, byte_size);
  cudaMalloc(&gpu_b, byte_size);
  cudaMalloc(&gpu_res, byte_size);

  cudaMemcpy(gpu_a, a, byte_size, cudaMemcpyHostToDevice);
  cudaMemcpy(gpu_b, b, byte_size, cudaMemcpyHostToDevice);

  dim3 threadsPerBlock(16, 16);
  dim3 numBlocks(SIZE / threadsPerBlock.x, SIZE / threadsPerBlock.y);
  matrix_mult<<<numBlocks, threadsPerBlock>>>(gpu_a, gpu_b, gpu_res);

  cudaDeviceSynchronize();

  cudaMemcpy(res, gpu_res, byte_size, cudaMemcpyDeviceToHost);

  cudaFree(gpu_a);
  cudaFree(gpu_b);
  cudaFree(gpu_res);
}
