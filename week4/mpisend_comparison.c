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

        // DIY: Send portions of the array to each process manually
        int elements_per_process = N / size;
        for (int i = 1; i < size; i++) {
            MPI_Send(array + i * elements_per_process, elements_per_process, MPI_INT, i, 0, MPI_COMM_WORLD);
        }

        // Root process gets its portion
        local_array = malloc(elements_per_process * sizeof(int));
        for (int i = 0; i < elements_per_process; i++) {
            local_array[i] = array[i];  // Copy its portion
        }
    } else {
        // Receive the portion of the array from root
        int elements_per_process = N / size;
        local_array = malloc(elements_per_process * sizeof(int));
        MPI_Recv(local_array, elements_per_process, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    // Perform local computation
    vector_addition(local_array, 0, N / size);

    // Root process collects results manually
    if (rank == 0) {
        // Root already has its result in local_array
        for (int i = 0; i < N / size; i++) {
            array[i] = local_array[i];
        }

        // Receive results from all other processes
        int elements_per_process = N / size;
        for (int i = 1; i < size; i++) {
            MPI_Recv(array + i * elements_per_process, elements_per_process, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        // Verify the results
        for (int i = 0; i < N; i++) {
            printf("%d ", array[i]);
        }
        printf("\n");

        free(array);
    } else {
        // Send the results back to the root process
        MPI_Send(local_array, N / size, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    free(local_array);
    MPI_Finalize();
    return 0;
}
