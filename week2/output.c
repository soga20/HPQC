#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Function to read a file and just read its contents without printing them
void read_file(const char *file_path) {
    FILE *file = fopen(file_path, "r");
    if (!file) {
        printf("Could not open file %s for reading\n", file_path);
        return;
    }

    // Just read through the file without printing its contents
    char ch;
    while ((ch = fgetc(file)) != EOF) {
        // Do nothing with 'ch' to avoid printing, just reading
    }

    fclose(file);
}

int main() {
    const char *file_path = "data/output.txt";  // Change this path as needed

    clock_t start_time = clock();  // Start timer
    read_file(file_path);  // Read the file
    clock_t end_time = clock();  // End timer

    double time_taken = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;  // Calculate time taken
    printf("\nTime taken to read the file: %.5f seconds\n", time_taken);

    return 0;
}

