import time

# Start timing
start_time = time.time()

# Generate some output
output = "Hello, this is a test file write.\n" * 100

# Write output to a file
with open("data/output.txt", "w") as file:
    file.write(output)

# End timing
end_time = time.time()

# Log execution time
with open("data/log.txt", "w") as log_file:
    log_file.write(f"Execution time: {end_time - start_time} seconds\n")
