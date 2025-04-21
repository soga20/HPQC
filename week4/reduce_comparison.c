#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define N 1000  // Size of the array

void vector_addition(int *vec, int start, int end) {
    for (int i = start; i < end; i++) {
        vec[i] += 1;  // Increment each element
    }
}

int main(int argc, char *argv[]) {
    int rank, size;
    int *array = NULL;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Allocate memory on all processes
    array = malloc(N * sizeof(int));
    if (array == NULL) {
        printf("Error: Unable to allocate memory on rank %d\n", rank);
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    // Initialize array only on root
    if (rank == 0) {
        for (int i = 0; i < N; i++) {
            array[i] = i;
        }
        printf("Array initialized on root process.\n");
    }

    // Broadcast array to all processes
    MPI_Bcast(array, N, MPI_INT, 0, MPI_COMM_WORLD);

    // Determine start and end index for this process
    int elements_per_process = N / size;
    int start = rank * elements_per_process;
    int end = (rank == size - 1) ? N : start + elements_per_process;

    // Perform local computation
    vector_addition(array, start, end);

    // Gather the modified array to root
    int *result = NULL;
    if (rank == 0) {
        result = malloc(N * sizeof(int));
    }

    MPI_Gather(&array[start], end - start, MPI_INT,
               result, end - start, MPI_INT,
               0, MPI_COMM_WORLD);

    // Print the final array on root
    if (rank == 0) {
        printf("Modified array gathered on root:\n");
        for (int i = 0; i < N; i++) {
            printf("%d ", result[i]);
        }
        printf("\n");
        free(result);
    }

    free(array);
    MPI_Finalize();
    return 0;
}
