#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define N 1000  // Size of full array

// Custom sum operation for arrays
void sum_op(void *in, void *inout, int *len, MPI_Datatype *dtype) {
    int *in_vals = (int *)in;
    int *inout_vals = (int *)inout;
    for (int i = 0; i < *len; i++) {
        inout_vals[i] += in_vals[i];
    }
}

int main(int argc, char *argv[]) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (N % size != 0) {
        if (rank == 0) {
            fprintf(stderr, "Array size N must be divisible by the number of processes.\n");
        }
        MPI_Finalize();
        return EXIT_FAILURE;
    }

    int chunk_size = N / size;
    int *array = NULL;
    int *local_array = malloc(chunk_size * sizeof(int));

    // Initialize full array on root
    if (rank == 0) {
        array = malloc(N * sizeof(int));
        for (int i = 0; i < N; i++) {
            array[i] = i + 1;  // Values 1 to N
        }
    }

    // Distribute the data
    MPI_Scatter(array, chunk_size, MPI_INT, local_array, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);

    // Create a custom MPI reduction operator
    MPI_Op custom_op;
    MPI_Op_create(sum_op, 1, &custom_op);

    // Allocate result array on root process
    int *result = NULL;
    if (rank == 0) {
        result = malloc(chunk_size * sizeof(int));
    }

    // Perform reduction using custom operator
    MPI_Reduce(local_array, result, chunk_size, MPI_INT, custom_op, 0, MPI_COMM_WORLD);

    // Output result on root
    if (rank == 0) {
        printf("Custom reduction result (partial sums):\n");
        for (int i = 0; i < chunk_size; i++) {
            printf("%d ", result[i]);
        }
        printf("\n");
        free(array);
        free(result);
    }

    // Cleanup
    MPI_Op_free(&custom_op);
    free(local_array);
    MPI_Finalize();
    return 0;
}
