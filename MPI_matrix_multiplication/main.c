#include <stdio.h>
#include <time.h>
#include <mpi.h>
#include "size.h"

float a[LEN];
float b[LEN];
float res[LEN];

extern void init_random(float[LEN]);
extern void init_zero(float[LEN]);
extern void matrix_mult(float[LEN], float[LEN], float[LEN], int, int);

int main(int argc, char**  argv) {
    // Initialize the MPI environment
    MPI_Init(&argc, &argv);

    // Get the number of processes and the rank of the process
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int executions = 0;
    clock_t total_clock = 0;
    while (executions < 10) {
        init_random(a);
        init_random(b);
        init_zero(res);

        // do multiplication
        clock_t start = clock();
        matrix_mult(a, b, res, rank, size);
        clock_t end = clock();

        executions++;
        total_clock += end - start;
    }
    printf("%f\n", (2.0 * SIZE * SIZE * SIZE * CLOCKS_PER_SEC * executions) / total_clock);

    // Finalize the MPI environment.
    MPI_Finalize();
}
