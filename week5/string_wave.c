#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

// Default values
#define DEFAULT_CYCLES 5
#define DEFAULT_SAMPLES 100
#define DEFAULT_OUTPUT "data/string_wave.csv"

// Struct to hold configuration parameters
typedef struct {
    int cycles;
    int samples;
    char output_location[256];
} Config;

// Function to parse command-line arguments
void check_args(int argc, char *argv[], Config *config) {
    // Set default values
    config->samples = DEFAULT_SAMPLES;
    config->cycles = DEFAULT_CYCLES;
    strcpy(config->output_location, DEFAULT_OUTPUT);

    // Override with user-provided arguments if available
    if (argc >= 2) {
        config->samples = atoi(argv[1]);
    }
    if (argc >= 3) {
        config->cycles = atoi(argv[2]);
    }
    if (argc >= 4) {
        strncpy(config->output_location, argv[3], sizeof(config->output_location) - 1);
        config->output_location[sizeof(config->output_location) - 1] = '\0'; // Null-terminate
    }
}

// Function to simulate the oscillation of the string and write to CSV
void simulate_wave(int cycles, int samples, const char *output_location) {
    FILE *file = fopen(output_location, "w");
    if (!file) {
        perror("Error opening output file");
        exit(1);
    }

    double *string = malloc(samples * sizeof(double));
    if (!string) {
        fprintf(stderr, "Memory allocation failed\n");
        fclose(file);
        exit(1);
    }

    int total_frames = cycles * 100;

    for (int t = 0; t < total_frames; t++) {
        for (int i = 0; i < samples; i++) {
            double x = (double)i / (samples - 1);
            string[i] = sin(2 * M_PI * x * cycles + 0.1 * t);
        }

        // Write the frame to the CSV file (no trailing comma)
        for (int i = 0; i < samples; i++) {
            fprintf(file, "%f", string[i]);
            if (i < samples - 1) fprintf(file, ",");
        }
        fprintf(file, "\n");
    }

    free(string);
    fclose(file);
}

int main(int argc, char *argv[]) {
    Config config;
    check_args(argc, argv, &config);

    // Display configuration
    printf("Cycles: %d, Samples: %d, Output Location: %s\n",
           config.cycles, config.samples, config.output_location);

    simulate_wave(config.cycles, config.samples, config.output_location);

    printf("Wave simulation complete.\n");
    return 0;
}
