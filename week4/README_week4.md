Part 1: MPI Communication Basics
Results
What happens with different numbers of processes?

The order of operations can vary, but that's normal for MPI.

Which send types performed better?

MPI_Isend() and MPI_Ssend() are generally reliable. MPI_Rsend() can give errors if the receiver is not ready, and MPI_Bsend() can behave unexpectedly if the buffer isn’t set up correctly.

Timing Results:

Timing was inconsistent, especially for very short messages. Variance in results can be greater than the actual communication times.

Part 2: Benchmarking Latency and Bandwidth
Results
Do results converge as the number of pings increases?

Yes, the latency tends to decrease with more pings, as the system stabilizes.

Results for Latency:

As we sent more pings, the average time per ping decreased.

Results for Bandwidth:

For larger messages, the time per byte was lower, showing better bandwidth with bigger message sizes.

Part 3: MPI Collective Communications
Results
Broadcast vs Scatter vs DIY:

MPI_Broadcast() and MPI_Scatter() are more efficient because they send data to multiple processes at once, as opposed to sending data individually. We predicted that collective operations would be faster, and our tests confirmed that.

Send/Recv vs Gather vs Reduce:

MPI_Gather() and MPI_Reduce() are more efficient than using MPI_Send() and MPI_Recv() because they handle data collection from multiple processes more efficiently. We found that MPI_Reduce() was faster in our tests.

Custom Reduce Operation:

Our custom reduce operation, created with MPI_Op_create(), worked as expected, but the predefined MPI_Reduce() was faster and more reliable.
