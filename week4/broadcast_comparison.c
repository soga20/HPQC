#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define N 1000  // size of the array

void vector_addition(int *vec, int start, int end) {
    for (int i = start; i < end; i++) {
        vec[i] += 1;  // Modify array elements
    }
}

int main(int argc, char *argv[]) {
    int rank, size;
    int *array = NULL;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Allocate memory for array in all processes
    array = malloc(N * sizeof(int));  

    // Initialize the array only on root (rank 0)
    if (rank == 0) {
        for (int i = 0; i < N; i++) {
            array[i] = i;  // Initialize the array
        }
    }

    // Step 1: Broadcast approach
    MPI_Bcast(array, N, MPI_INT, 0, MPI_COMM_WORLD);  // Broadcasting the entire array to all processes

    int elements_per_process = N / size;
    int start = rank * elements_per_process;
    int end = (rank == size - 1) ? N : start + elements_per_process;

    vector_addition(array, start, end);  // Perform local computation

    // Gather results back at root (if necessary)
    MPI_Gather(array + start, elements_per_process, MPI_INT, array, elements_per_process, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        // Verify the results
        for (int i = 0; i < N; i++) {
            printf("%d ", array[i]);
        }
        printf("\n");
        free(array);
    }

    MPI_Finalize();
    return 0;
}
