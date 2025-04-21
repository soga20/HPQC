#include <stdio.h>
#include <mpi.h>

void send_message_to_root(int rank, int size);
void receive_messages_from_others(int rank, int size);

int main(int argc, char **argv) {
    int rank, size;
    MPI_Init(&argc, &argv);  // Init
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    printf("Rank %d starting communication with size %d\n", rank, size);
    fflush(stdout);  // Make sure output prints

    // Make sure everyone reaches the same point
    MPI_Barrier(MPI_COMM_WORLD);

    if (size < 2) {
        if (rank == 0)
            printf("Need at least 2 processes to run.\n");
        MPI_Finalize();
        return 0;
    }

    if (rank == 0) {
        receive_messages_from_others(rank, size);
    } else {
        send_message_to_root(rank, size);
    }

    MPI_Finalize();  // Done
    return 0;
}

void send_message_to_root(int rank, int size) {
    int message = rank * 10;
    MPI_Request req;

    double start = MPI_Wtime();
    MPI_Isend(&message, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &req);
    MPI_Wait(&req, MPI_STATUS_IGNORE);
    double end = MPI_Wtime();

    printf("Hello, I am %d of %d. Sent %d to Rank 0.\n", rank, size, message);
    printf("[Rank %d] Took %f seconds to send the message.\n", rank, end - start);
    fflush(stdout);
}

void receive_messages_from_others(int rank, int size) {
    int message;
    MPI_Status status;

    double start = MPI_Wtime();
    for (int src = 1; src < size; src++) {
        MPI_Recv(&message, 1, MPI_INT, src, 0, MPI_COMM_WORLD, &status);
        printf("Hello, I am %d of %d. Received %d from Rank %d.\n", rank, size, message, src);
        fflush(stdout);
    }
    double end = MPI_Wtime();

    printf("[Rank %d] Took %f seconds to receive all messages.\n", rank, end - start);
    fflush(stdout);
}
