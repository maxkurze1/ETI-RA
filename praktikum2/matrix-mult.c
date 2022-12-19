#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "size.h"


double *A; // full matrix for master process only
double *B;

int MASTER;
int rank;
int num_proc;

void random_init(double *mat, int rows, int cols);
void print_arr(double* arr, int size);
void print_matrix(double *mat, int rows, int cols);

int main(int argc, char **argv){
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &num_proc);
  int workers = num_proc - 1;
  MASTER = num_proc - 1;

  // calculate block size
  int count = SIZE/sqrt(num_proc);

  if (rank == MASTER) {
    // master process creates matrix and scatters values
    MPI_Alloc_mem(sizeof(double) * SIZE * SIZE, MPI_INFO_NULL, &A);
    MPI_Alloc_mem(sizeof(double) * SIZE * SIZE, MPI_INFO_NULL, &B);

    // random init on master process
    srand(time(0));
    random_init(A, SIZE, SIZE);
    random_init(B, SIZE, SIZE);

    #ifdef debug
    print_matrix(A, SIZE, SIZE);
    print_matrix(B, SIZE, SIZE);
    #endif

    // scatter
    MPI_Request send[workers*2];
    for (int i = 0; i < workers; i++) {
      MPI_Isend(&A[count * SIZE * i], count * SIZE, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, &send[i*2]);
      MPI_Isend(&B[count * SIZE * i], count * SIZE, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, &send[i*2+1]);
    }
    MPI_Waitall(workers * 2, send, MPI_STATUSES_IGNORE);
  } else {
    // alloc memory
    MPI_Alloc_mem(sizeof(double) * count * SIZE, MPI_INFO_NULL, &A);
    MPI_Alloc_mem(sizeof(double) * count * SIZE, MPI_INFO_NULL, &B);

    // receive matrix part
    MPI_Request recv[2];
    MPI_Irecv(A, count * SIZE, MPI_DOUBLE, MASTER, 0, MPI_COMM_WORLD, &recv[0]);
    MPI_Irecv(A, count * SIZE, MPI_DOUBLE, MASTER, 0, MPI_COMM_WORLD, &recv[1]);
    MPI_Waitall(2, recv, MPI_STATUSES_IGNORE);

    int startA = count * SIZE * rank;
    int startB = count * SIZE * rank;
    for (int x = 0; x < SIZE; x++){
      for (int y = 0; y < SIZE)
    }
  }


  // MPI_Scatter(A, SIZE, MPI_DOUBLE, part, SIZE, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  

  // print each part
  #ifdef debug
  if (i == 0)
    print_matrix(A, SIZE * SIZE);

  MPI_Barrier(MPI_COMM_WORLD);

  for (int i = 1; i < num_proc; i ++) {
    if (i == rank) {
      print_matrix(A, count, SIZE);
    }
    MPI_Barrier(MPI_COMM_WORLD);
  }
  #endif

  MPI_Finalize();

  return 0;
}

void random_init(double *mat, int rows, int cols){
  srand(time(0));
  for (int total_size = rows * cols, x = 0; x < total_size; x ++) {
      mat[x] = (double)rand();
  }
}

void print_arr(double *arr, int size){
  printf("[");
  for (int i = 0;i < size; i++) {
    if (i != 0)
      printf(" ");
    printf("%#g", arr[i]);
  }
  printf("]\n\n");
}

void print_matrix(double *mat, int rows, int cols){
  for (int x = 0; x < cols * rows; x += cols) {
    if (x == 0)
      printf("[");
    else
      printf(" ");
    for (int y = 0; y < cols; y ++) {
      if (y != 0)
        printf(" ");
      printf("%#g", mat[x +y]);
    }
    if (x == cols * (rows-1))
      printf("]\n");
    else
      printf(" \n");
  }
  printf("\n");
}
