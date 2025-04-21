Purpose
This repository contains code that simulates a vibrating string using a simplistic model of oscillations. The project involves two main components:

string_wave.c: A C program that simulates the oscillations of a string using basic wave physics. The program outputs data for each time step and writes it to a CSV file.

animate_line_file.py: A Python program used to read the generated CSV file from the C program and create an animated plot that visualizes the string's oscillations.

Additionally, the project involves the parallelization of the simulation using MPI (Message Passing Interface) to scale the simulation for larger systems.

Step 1: String Simulation in C (string_wave.c)
Key Updates
Removed Hardcoding: The number of points, cycles, and output file location were previously hardcoded. Now, they can be specified via command-line arguments.

Enhanced Documentation: The code is well-documented with comments explaining the purpose of each section.

Step 2: Animated Visualization (animate_line_file.py)
Key Updates
Improved Flexibility: The Python script is now able to read different CSV files and visualize them, instead of being hardcoded for a specific file.

Code Refactoring: The script was refactored to handle different data files and provided proper documentation for future usage.

Step 3: Parallelization with MPI (string_wave_parallel.c)
Key Parallelization Strategy
The string is split into chunks, with each process handling a portion of the string. Each process calculates the wave evolution independently for its chunk.

Neighboring processes communicate with each other to manage the interaction between the edges of their chunks.

After the computations are done, the results are gathered from all processes and combined to produce the final simulation data.

Key Updates
MPI Parallelization: The simulation was parallelized using MPI to distribute the computational load across multiple processes.

Communication Between Processes: Neighboring processes communicate their boundary data to ensure accurate results at the string's edges.

Benchmarking: The performance of the serial and parallel versions was benchmarked to evaluate the scalability of the solution.

Step 4: Model Improvement (Freeform Exercise)
Key Improvements
Spring Force: Each point now experiences a force based on its displacement from equilibrium, adding a more physically accurate interaction between points.

Velocity and Acceleration: Each point has a velocity and acceleration, improving the realism of the wave propagation.

