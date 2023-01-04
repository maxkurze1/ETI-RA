#include "size.h"
#include <mpi.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

int rank;
int num_proc;
int MASTER;

double *A; // master matrix A
double *B; // master matrix B
double *C; // master result C = A * B

double start;
double end;

void random_init(double *mat, int rows, int cols);

int main(int argc, char **argv) {
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &num_proc);
  MASTER = 0;

  B = malloc(sizeof(double) * LEN);
  if (rank == MASTER) {
    // only MASTER-Process allocates memory for the full A & B matrix
    A = malloc(sizeof(double) * LEN);

    // init A & B only on MASTER
    srand(time(NULL));
    random_init(A, SIZE, SIZE);
    random_init(B, SIZE, SIZE);
  }

  MPI_Barrier(MPI_COMM_WORLD);
  start = MPI_Wtime();

  // array describing how many elements to send to each process
  int *elements_per_process = malloc(
      num_proc * sizeof(int)); // name is a bit misleading as it really stores
                               // the number of elements per process
  // array describing the displacements where each segment begins
  int *displacement = malloc(num_proc * sizeof(int));

  // Calculate displacements for Matrix A (rows)
  int dis = 0;
  for (int i = 0; i < num_proc; i++) {
    elements_per_process[i] = ((SIZE / num_proc) + (i < (SIZE % num_proc))) * SIZE;
    displacement[i] = dis;
    dis += elements_per_process[i];
  }

  // Send/receive matrix B
  MPI_Bcast(B, LEN, MPI_DOUBLE, MASTER, MPI_COMM_WORLD);

  // Send rows of matrix A to processes
  int row_count = (SIZE / num_proc) + (rank < (SIZE % num_proc));
  double *rec_rows_buffer = malloc(row_count * SIZE * sizeof(double));

  MPI_Scatterv(A, elements_per_process, displacement, MPI_DOUBLE, rec_rows_buffer,
               row_count * SIZE, MPI_DOUBLE, MASTER, MPI_COMM_WORLD);

  // Calculate results
  double *imm_res = malloc(row_count * SIZE * sizeof(double));
  for (int row = 0; row < row_count; row++) {
    for (int idx_a = 0; idx_a < SIZE; idx_a++) {
      double sum = 0;
      for (int idx_b = 0; idx_b < SIZE; idx_b++) {
        sum += rec_rows_buffer[row * SIZE + idx_b] * B[idx_b * SIZE + idx_a];
      }
      imm_res[row * SIZE + idx_a] = sum;
    }
  }

  if (rank == MASTER)
    C = malloc(LEN * sizeof(double));

  MPI_Gatherv(imm_res, row_count * SIZE, MPI_DOUBLE, C, elements_per_process,
              displacement, MPI_DOUBLE, MASTER, MPI_COMM_WORLD);

  MPI_Barrier(MPI_COMM_WORLD);
  end = MPI_Wtime();
  if (rank == MASTER) {
    printf("%f\n", (SIZE * SIZE * (2.0 * SIZE - 1))  / (end - start));

    free(A);
    free(C);
  }

  // Free memory
  free(B);
  free(elements_per_process);
  free(rec_rows_buffer);
  free(displacement);
  free(imm_res);
  MPI_Finalize();
}

void random_init(double *mat, int rows, int cols) {
  for (int total_size = rows * cols, i = 0; i < total_size; i++)
    mat[i] = (double)rand();
}
