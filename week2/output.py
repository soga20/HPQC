import time

# Function to read a file and return its contents
def read_file(file_path):
    with open(file_path, 'r') as file:
        return file.read()

# Time how long it takes to read the file
def time_file_read(file_path):
    start_time = time.time()  # Start timer
    file_contents = read_file(file_path)  # Read the file
    end_time = time.time()  # End timer
    
    time_taken = end_time - start_time  # Calculate time taken
    print(f"Time taken to read the file: {time_taken} seconds")
    
    return file_contents  # Return contents in case you want to process it

# Example file path
file_path = 'data/output.txt'  # Change this path if needed

# Run the function and see how long it takes
contents = time_file_read(file_path)

# Optionally, print the contents if needed (for small files)
# print(contents)

