#include "size.h"
#include <mpi.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

int rank;
int num_proc;
int MASTER;
void random_init(double *mat, int rows, int cols);
void print_arr(double *arr, int size);
void print_matrix(double *mat, int rows, int cols);

double *A; // master matrix A
double *B; // master matrix B
double *C; // master result C = A * B

double start;
double end;

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

    // printf("A %p:\n", A);
    // print_matrix(A, SIZE, SIZE);
    // printf("B %p:\n", B);
    // print_matrix(B, SIZE, SIZE);

    // correct_c = malloc(sizeof(double) * LEN);
    //  for (int r = 0; r < SIZE; r++)
    //   for (int c = 0; c < SIZE; c++)
    //     for (int i = 0; i < SIZE; i++)
    //       correct_c[r * SIZE + c] += A[r * SIZE + i] * B[i * SIZE + c];

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

  // for (int i = 0; i < num_proc; i++) {
  //   if (rank == i) {
  //     printf("Process %d:\n", rank);
  //     print_matrix(rec_rows_buffer, row_count, SIZE);
  //   }
  //   MPI_Barrier(MPI_COMM_WORLD);
  // }

  // Calculate results
  double *imm_res;
  imm_res = malloc(row_count * SIZE * sizeof(double));
  for (int row = 0; row < row_count; row++) {
    for (int idx_a = 0; idx_a < SIZE; idx_a++) {
      double sum = 0;
      for (int idx_b = 0; idx_b < SIZE; idx_b++) {
        sum += rec_rows_buffer[row * SIZE + idx_b] * B[idx_b * SIZE + idx_a];
      }
      imm_res[row * SIZE + idx_a] = sum;
    }
  }

  // for (int i = 0; i < num_proc; i++) {
  //   if (rank == i) {
  //     printf("Process %d res:\n", rank);
  //     print_matrix(imm_res, row_count, SIZE);
  //   }
  //   MPI_Barrier(MPI_COMM_WORLD);
  // }

  if (rank == MASTER)
    C = malloc(LEN * sizeof(double));

  MPI_Gatherv(imm_res, row_count * SIZE, MPI_DOUBLE, C, elements_per_process,
              displacement, MPI_DOUBLE, MASTER, MPI_COMM_WORLD);

  MPI_Barrier(MPI_COMM_WORLD);
  end = MPI_Wtime();
  if (rank == MASTER) {
    printf("%f\n", (SIZE * SIZE * (2.0 * SIZE - 1))  / (end - start));
  }

  // if (rank == MASTER) {
  //   printf("Result:\n");
  //   print_matrix(C, SIZE, SIZE);

  //   for (int i = 0; i < LEN; i++)
  //     if (correct_c[i] != C[i])
  //       printf("Wrong result at (%d): expected (%lf) got (%lf)\n", i, correct_c[i], C[i]);

  // }

  // Free memory
  // free(rec_rows_buffer);
  // free(displacement);
  // free(rows_per_process);
  MPI_Finalize();
}

void random_init(double *mat, int rows, int cols) {
  for (int total_size = rows * cols, i = 0; i < total_size; i++)
    mat[i] = (double)rand();
}

void print_arr(double *arr, int size) {
  printf("[");
  for (int i = 0; i < size; i++) {
    if (i != 0)
      printf(" ");
    printf("%#g", arr[i]);
  }
  printf("]\n\n");
}

void print_matrix(double *mat, int rows, int cols) {
  for (int x = 0; x < cols * rows; x += cols) {
    if (x == 0)
      printf("[");
    else
      printf(" ");
    for (int y = 0; y < cols; y++) {
      if (y != 0)
        printf(" ");
      printf("%#g", mat[x + y]);
    }
    if (x == cols * (rows - 1))
      printf("]\n");
    else
      printf(" \n");
  }
  printf("\n");
}
