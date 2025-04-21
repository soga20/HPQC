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
    int *local_array = NULL;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Initialize the array only on root (rank 0)
    if (rank == 0) {
        array = malloc(N * sizeof(int));        
        for (int i = 0; i < N; i++) {
            array[i] = i;  // Initialize the array
        }
    }

    // Step 2: Scatter approach
    int elements_per_process = N / size;
    local_array = malloc(elements_per_process * sizeof(int));

    // Scatter the array from root to all processes
    MPI_Scatter(array, elements_per_process, MPI_INT, local_array, elements_per_process, MPI_INT, 0, MPI_COMM_WORLD);

    // Perform local computation
    vector_addition(local_array, 0, elements_per_process);

    // Gather results back at root (if necessary)
    MPI_Gather(local_array, elements_per_process, MPI_INT, array, elements_per_process, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        // Verify the results
        for (int i = 0; i < N; i++) {
            printf("%d ", array[i]);
        }
        printf("\n");
        free(array);
    }

    free(local_array);
    MPI_Finalize();
    return 0;
}
