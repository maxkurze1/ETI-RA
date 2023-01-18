#include "default.h"
#include <cstdio>

__global__ void matrix_mult(double *a, double *b, double *res) {
  for (int r = blockIdx.x * blockDim.x + threadIdx.x; r < SIZE; r += blockDim.x * gridDim.x)
    for (int c = blockIdx.y * blockDim.y + threadIdx.y; c < SIZE; c += blockDim.y * gridDim.y)
      for (int i = 0; i < SIZE; i++)
        res[r * SIZE + c] += a[r * SIZE + i] * b[i * SIZE + c];
}

void cuda_matrix_mult(double a[SIZE][SIZE], double b[SIZE][SIZE], double res[SIZE][SIZE]) {
  double *gpu_a;
  double *gpu_b;
  double *gpu_res;

  constexpr size_t byte_size = SIZE * SIZE * sizeof(double);

  cudaMallocManaged(&gpu_a, byte_size);
  cudaMallocManaged(&gpu_b, byte_size);
  cudaMallocManaged(&gpu_res, byte_size);

  // cudaMemcpy(gpu_a, a, byte_size, cudaMemcpyHostToDevice);
  // cudaMemcpy(gpu_b, b, byte_size, cudaMemcpyHostToDevice);
  for (int i = 0; i < SIZE * SIZE; i++) {
    gpu_a[i] = ((double*)a)[i];
    gpu_b[i] = ((double*)b)[i];
  }
  
  int deviceID;
  cudaGetDevice(&deviceID);

  cudaMemPrefetchAsync(gpu_a, byte_size, deviceID);
  cudaMemPrefetchAsync(gpu_b, byte_size, deviceID);

  dim3 threadsPerBlock(1, 128);
  dim3 numBlocks(SIZE/(threadsPerBlock.x), SIZE/(threadsPerBlock.y));
  matrix_mult<<<numBlocks, threadsPerBlock>>>(gpu_a, gpu_b, gpu_res);

  cudaDeviceSynchronize();

  cudaMemPrefetchAsync(gpu_res, byte_size, deviceID);

  for (int i = 0; i < SIZE * SIZE; i++){
    ((double*)res)[i] = gpu_res[i];
  }
  // cudaMemcpy(res, gpu_res, byte_size, cudaMemcpyDeviceToHost);

  cudaFree(gpu_a);
  cudaFree(gpu_b);
  cudaFree(gpu_res);
}
