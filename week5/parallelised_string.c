#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define N 1000  // Total number of points on the string
#define CYCLES 5  // Number of oscillation cycles
#define OUTPUT_FILE "data/string_wave_parallel.csv"

// Function to update the positions of the points on the string
void update_positions(double* current, double* previous, double* next, int chunk_size) {
    for (int i = 1; i < chunk_size - 1; i++) {
        next[i] = 2 * current[i] - previous[i] + (current[i-1] - 2 * current[i] + current[i+1]);
    }
}

// Main function
int main(int argc, char *argv[]) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Ensure the total number of points is divisible by the number of processes
    int chunk_size = N / size;

    // Allocate memory for local chunks (current and previous states)
    double* local_current = (double*) malloc(chunk_size * sizeof(double));
    double* local_previous = (double*) malloc(chunk_size * sizeof(double));
    double* local_next = (double*) malloc(chunk_size * sizeof(double));

    // Initialize the string wave (simplified)
    if (rank == 0) {
        // Root process initializes the wave
        for (int i = 0; i < N; i++) {
            if (i == N / 2) { // Displacement in the middle
                local_current[i % chunk_size] = 1.0;
            } else {
                local_current[i % chunk_size] = 0.0;
            }
            local_previous[i % chunk_size] = local_current[i % chunk_size];
        }
    }

    // Broadcast the initial condition to all processes
    MPI_Bcast(local_current, chunk_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(local_previous, chunk_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Perform the simulation
    for (int cycle = 0; cycle < CYCLES; cycle++) {
        // Update the positions of the points
        update_positions(local_current, local_previous, local_next, chunk_size);

        // Exchange boundary points with neighboring processes
        if (rank > 0) {  // Send to the previous process
            MPI_Send(&local_current[1], 1, MPI_DOUBLE, rank - 1, 0, MPI_COMM_WORLD);
            MPI_Recv(&local_current[0], 1, MPI_DOUBLE, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        if (rank < size - 1) {  // Send to the next process
            MPI_Send(&local_current[chunk_size - 2], 1, MPI_DOUBLE, rank + 1, 0, MPI_COMM_WORLD);
            MPI_Recv(&local_current[chunk_size - 1], 1, MPI_DOUBLE, rank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        // Update previous and current for next iteration
        double* temp = local_previous;
        local_previous = local_current;
        local_current = local_next;
        local_next = temp;
    }

    // Gather the results in the root process
    double* global_wave = NULL;
    if (rank == 0) {
        global_wave = (double*) malloc(N * sizeof(double));
    }

    // Gather the data from all processes
    MPI_Gather(local_current, chunk_size, MPI_DOUBLE, global_wave, chunk_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Write the result to a CSV file in the root process
    if (rank == 0) {
        FILE* file = fopen(OUTPUT_FILE, "w");
        if (file != NULL) {
            for (int i = 0; i < N; i++) {
                fprintf(file, "%d,%lf\n", i, global_wave[i]);
            }
            fclose(file);
            printf("Data written to %s\n", OUTPUT_FILE);
        } else {
            printf("Error opening output file\n");
        }
        free(global_wave);
    }

    // Free memory and finalize MPI
    free(local_current);
    free(local_previous);
    free(local_next);
    MPI_Finalize();
    return 0;
}
