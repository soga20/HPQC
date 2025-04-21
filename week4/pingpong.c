#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void pingpong(int num_pings);

int main(int argc, char **argv) {
    int rank, size;
    
    // Initialize MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Ensure only two processes are used
    if (size != 2) {
        printf("This program requires exactly two processes.\n");
        MPI_Finalize();
        return 1;
    }

    // Get number of pings from command line arguments
    int num_pings = atoi(argv[1]);

    // Run the pingpong operation
    pingpong(num_pings);

    MPI_Finalize();
    return 0;
}

void pingpong(int num_pings) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    int counter = 0;
    double start_time, end_time;
    double elapsed_time, average_time;

    if (rank == 0) { // Root process (sender)
        start_time = MPI_Wtime();
        
        for (counter = 0; counter < num_pings; counter++) {
            MPI_Send(&counter, 1, MPI_INT, 1, 0, MPI_COMM_WORLD); // Send to client
            MPI_Recv(&counter, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // Receive from client
        }

        end_time = MPI_Wtime();
        elapsed_time = end_time - start_time;
        average_time = elapsed_time / num_pings;

        printf("Ping-Pong completed. Total pings: %d\n", num_pings);
        printf("Elapsed Time: %f seconds\n", elapsed_time);
        printf("Average Time per Ping: %f seconds\n", average_time);

    } else if (rank == 1) { // Client process (receiver)
        for (counter = 0; counter < num_pings; counter++) {
            MPI_Recv(&counter, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // Receive from root
            counter++; // Increment counter
            MPI_Send(&counter, 1, MPI_INT, 0, 0, MPI_COMM_WORLD); // Send back to root
        }
    }
}
