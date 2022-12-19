#include <malloc.h>
#include <mpi.h>
#include <stdlib.h>
#include <time.h>
#include "size.h"

int rank;
int num_proc;
int MASTER;
void random_init(double *mat, int rows, int cols);

double *A; // matrix A
double *B; // matrix B
double *C; // C = A * B

int main(int argc, char **argv) {
// extern void matrix_mult(float a[LEN], float b[LEN], float res[LEN], int rank, int size) {
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_proc);
    MASTER = 0;

    if (rank == MASTER) {
      // only MASTER-Process allocates memory for the full A & B matrix
      A = malloc(sizeof(double) * LEN);
      B = malloc(sizeof(double) * LEN);

      // init A & B only on MASTER
      srand(time(0));
      random_init(A, SIZE, SIZE);
      random_init(B, SIZE, SIZE);
    }

    // array describing how many elements to send to each process
    int *rows_per_process = malloc(num_proc * sizeof(int)); // name is a bit misleading as it really stores the number of elements per process
    // array describing the displacements where each segment begins
    int *displacement = malloc(num_proc * sizeof(int));

    // Calculate displacements for Matrix A (rows)
    int dis = 0;
    for (int i = 0; i < num_proc; i++) {
        rows_per_process[i] = ((SIZE / num_proc) + (i < (SIZE % num_proc))) * SIZE;
        displacement[i] = dis;
        dis += rows_per_process[i];
    }

    double* buff_b;
    buff_b = malloc(LEN * sizeof(double));

    // Send data to other processes
    if (rank == MASTER)
        buff_b = B; // Set own buffer for matrix B to b

    // Send/receive matrix B
    MPI_Bcast(buff_b, LEN, MPI_DOUBLE, MASTER, MPI_COMM_WORLD);

    // Send rows of matrix A to processes
    int row_count = (SIZE / num_proc) + (rank < (SIZE % num_proc));
    float *rec_rows_buffer;
    rec_rows_buffer = malloc(row_count * SIZE * sizeof(double));

    MPI_Scatterv(A, rows_per_process, displacement, MPI_DOUBLE, rec_rows_buffer, row_count * SIZE, MPI_DOUBLE, MASTER, MPI_COMM_WORLD);

    // Calculate results
    double *imm_res;
    imm_res = malloc(row_count*SIZE*sizeof(double));
    for (int row = 0; row < row_count; row++) {
        for (int idx_a = 0; idx_a < SIZE; idx_a++) {
            double sum = 0;
            for (int idx_b = 0; idx_b < SIZE; idx_b++) {
                sum += rec_rows_buffer[row * row_count + idx_b] * B[idx_b * SIZE + idx_a];
            }
            imm_res[row * row_count + idx_a] = sum;
        }
    }

    double *result;
    result = malloc(LEN*sizeof(double));

    MPI_Gatherv(imm_res, row_count * SIZE, MPI_DOUBLE, result, rows_per_process, displacement, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (rank == 0) { // Write results to 'res'
        for (int i = 0; i < LEN; i++) {
            C[i] = result[i];
        }
    }

    // Free memory
    free(rec_rows_buffer);
    free(displacement);
    free(rows_per_process);
}

void random_init(double *mat, int rows, int cols){
  srand(time(0));
  for (int total_size = rows * cols, x = 0; x < total_size; x ++)
      mat[x] = (double) rand();
}
