#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void pingpong_bandwidth(int num_pings, int array_size);

int main(int argc, char **argv) {
    int rank, size;

    // Initialize MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Ensure exactly two processes
    if (size != 2) {
        printf("This program requires exactly two processes.\n");
        MPI_Finalize();
        return 1;
    }

    // Get number of pings and array size from command line arguments
    int num_pings = atoi(argv[1]);
    int array_size = atoi(argv[2]);

    // Run the bandwidth benchmarking
    pingpong_bandwidth(num_pings, array_size);

    MPI_Finalize();
    return 0;
}

void pingpong_bandwidth(int num_pings, int array_size) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Create an array of given size
    char *send_data = (char *)malloc(array_size * sizeof(char));
    char *recv_data = (char *)malloc(array_size * sizeof(char));
    double start_time, end_time, elapsed_time, average_time;

    // Initialize the array with data (optional)
    for (int i = 0; i < array_size; i++) {
        send_data[i] = (char)(i % 256); // Simple data pattern
    }

    if (rank == 0) { // Root process (sender)
        start_time = MPI_Wtime();

        for (int counter = 0; counter < num_pings; counter++) {
            MPI_Send(send_data, array_size, MPI_BYTE, 1, 0, MPI_COMM_WORLD); // Send data
            MPI_Recv(recv_data, array_size, MPI_BYTE, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // Receive data
        }

        end_time = MPI_Wtime();
        elapsed_time = end_time - start_time;
        average_time = elapsed_time / num_pings;

        // Calculate bandwidth in bytes per second
        double bandwidth = (array_size * num_pings) / elapsed_time;
        printf("Bandwidth Test\n");
        printf("Elapsed Time: %f seconds\n", elapsed_time);
        printf("Average Time per Ping: %f seconds\n", average_time);
        printf("Bandwidth: %f bytes/sec\n", bandwidth);
    } else if (rank == 1) { // Client process (receiver)
        for (int counter = 0; counter < num_pings; counter++) {
            MPI_Recv(recv_data, array_size, MPI_BYTE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // Receive data
            MPI_Send(send_data, array_size, MPI_BYTE, 0, 0, MPI_COMM_WORLD); // Send back data
        }
    }

    free(send_data);
    free(recv_data);
}
