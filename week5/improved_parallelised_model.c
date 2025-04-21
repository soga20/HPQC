#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

#define N 1000      // Total number of points on the string
#define CYCLES 1000 // Number of simulation steps
#define K 0.1       // Spring constant
#define M 0.1       // Mass of each point
#define GAMMA 0.01  // Damping coefficient
#define DT 0.01     // Time step

typedef struct {
    double position;
    double velocity;
    double acceleration;
} Point;

int main(int argc, char *argv[]) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (N % size != 0) {
        if (rank == 0)
            fprintf(stderr, "Error: N must be divisible by number of processes.\n");
        MPI_Finalize();
        return EXIT_FAILURE;
    }

    int chunk_size = N / size;
    Point* local_data = (Point*)calloc(chunk_size, sizeof(Point));
    Point* global_data = NULL;

    if (rank == 0) {
        global_data = (Point*)calloc(N, sizeof(Point));
        for (int i = 0; i < N; i++) {
            global_data[i].position = (i == N / 2) ? 1.0 : 0.0;
            global_data[i].velocity = 0.0;
            global_data[i].acceleration = 0.0;
        }
    }

    // Distribute initial data to all processes
    MPI_Scatter(global_data, chunk_size * sizeof(Point), MPI_BYTE,
                local_data, chunk_size * sizeof(Point), MPI_BYTE, 0, MPI_COMM_WORLD);

    for (int cycle = 0; cycle < CYCLES; cycle++) {
        // Boundary ghost points
        Point left_neighbor = local_data[0];
        Point right_neighbor = local_data[chunk_size - 1];

        // Communicate ghost cells
        if (rank != 0) {
            MPI_Sendrecv(&local_data[0], sizeof(Point), MPI_BYTE, rank - 1, 0,
                         &left_neighbor, sizeof(Point), MPI_BYTE, rank - 1, 1,
                         MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        if (rank != size - 1) {
            MPI_Sendrecv(&local_data[chunk_size - 1], sizeof(Point), MPI_BYTE, rank + 1, 1,
                         &right_neighbor, sizeof(Point), MPI_BYTE, rank + 1, 0,
                         MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        // Temporarily insert ghost cells
        Point* extended = (Point*) malloc((chunk_size + 2) * sizeof(Point));
        extended[0] = left_neighbor;
        for (int i = 0; i < chunk_size; i++) {
            extended[i + 1] = local_data[i];
        }
        extended[chunk_size + 1] = right_neighbor;

        // Update using extended buffer
        for (int i = 1; i < chunk_size + 1; i++) {
            double spring_force_left = K * (extended[i - 1].position - extended[i].position);
            double spring_force_right = K * (extended[i + 1].position - extended[i].position);
            double net_force = spring_force_left + spring_force_right;
            double damping_force = GAMMA * extended[i].velocity;

            extended[i].acceleration = (net_force - damping_force) / M;
            extended[i].velocity += extended[i].acceleration * DT;
            extended[i].position += extended[i].velocity * DT;
        }

        // Copy back updated data (ignore ghost cells)
        for (int i = 0; i < chunk_size; i++) {
            local_data[i] = extended[i + 1];
        }

        free(extended);
    }

    // Gather final data
    if (rank == 0) {
        global_data = (Point*) realloc(global_data, N * sizeof(Point)); // Already allocated
    }
    MPI_Gather(local_data, chunk_size * sizeof(Point), MPI_BYTE,
               global_data, chunk_size * sizeof(Point), MPI_BYTE,
               0, MPI_COMM_WORLD);

    // Write to CSV
    if (rank == 0) {
        FILE* file = fopen("string_wave_parallel.csv", "w");
        if (file) {
            for (int i = 0; i < N; i++) {
                fprintf(file, "%d,%lf,%lf,%lf\n", i,
                        global_data[i].position,
                        global_data[i].velocity,
                        global_data[i].acceleration);
            }
            fclose(file);
            printf("Output written to string_wave_parallel.csv\n");
        } else {
            fprintf(stderr, "Failed to open output file.\n");
        }
        free(global_data);
    }

    free(local_data);
    MPI_Finalize();
    return 0;
}
