#include <stdio.h>
#include <time.h>

int main() {
    clock_t start, end;
    double cpu_time_used;
    
    start = clock();  // Start timing
    
    FILE *file = fopen("data/output.txt", "w");  // Open file for writing
    if (file == NULL) {
        printf("Error opening file!\n");
        return 1;
    }

    // Write output to file
    for (int i = 0; i < 100; i++) {
        fprintf(file, "Hello, this is a test file write.\n");
    }
    
    fclose(file);  // Close file

    end = clock();  // End timing
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    // Log execution time
    FILE *log_file = fopen("data/log.txt", "w");
    if (log_file != NULL) {
        fprintf(log_file, "Execution time: %f seconds\n", cpu_time_used);
        fclose(log_file);
    }

    return 0;
}
