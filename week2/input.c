#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Function to read the contents of a file into memory
char *read_file(const char *file_path, size_t *size) {
    FILE *file = fopen(file_path, "r");
    if (file == NULL) {
        fprintf(stderr, "Could not open file %s for reading\n", file_path);
        return NULL;
    }

    // Get the size of the file
    fseek(file, 0, SEEK_END);
    *size = ftell(file);  // Get the size in bytes
    fseek(file, 0, SEEK_SET);  // Move back to the start of the file

    // Allocate memory for the file contents
    char *buffer = (char *)malloc(*size + 1);  // +1 for the null terminator
    if (buffer == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        fclose(file);
        return NULL;
    }

    // Read the contents into memory
    fread(buffer, 1, *size, file);
    buffer[*size] = '\0';  // Null-terminate the string

    fclose(file);
    return buffer;
}

int main() {
    const char *file_path = "data/output.txt";  // Change to your file path if needed
    size_t size = 0;

    // Start timing
    clock_t start_time = clock();

    // Read the file
    char *file_contents = read_file(file_path, &size);

    // Stop timing
    clock_t end_time = clock();

    // Calculate the time taken
    double time_taken = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

    if (file_contents != NULL) {
        printf("Time taken to read the file: %.5f seconds\n", time_taken);

        // Optionally, print the contents for small files
        // printf("%s", file_contents);

        free(file_contents);  // Don't forget to free the memory
    }

    return 0;
}

