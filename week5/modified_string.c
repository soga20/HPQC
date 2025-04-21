#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define DEFAULT_CYCLES 5
#define DEFAULT_SAMPLES 100
#define DEFAULT_OUTPUT "data/string_wave.csv"

typedef struct {
    int cycles;
    int samples;
    char output_location[256];
} Config;

void check_args(int argc, char *argv[], Config *config) {
    config->samples = (argc >= 2) ? atoi(argv[1]) : DEFAULT_SAMPLES;
    config->cycles = (argc >= 3) ? atoi(argv[2]) : DEFAULT_CYCLES;
    strcpy(config->output_location, (argc >= 4) ? argv[3] : DEFAULT_OUTPUT);
}

void simulate_wave(int cycles, int samples, const char *output_location) {
    FILE *file = fopen(output_location, "w");
    if (!file) {
        perror("Error opening output file");
        exit(1);
    }

    double *string = malloc(samples * sizeof(double));

    for (int t = 0; t < cycles * 100; t++) {
        for (int i = 0; i < samples; i++) {
            double x = (double)i / (samples - 1);
            string[i] = sin(2 * M_PI * (x - 0.5) * cycles + 0.1 * t);
        }

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
    printf("Cycles: %d, Samples: %d, Output Location: %s\n", config.cycles, config.samples, config.output_location);
    simulate_wave(config.cycles, config.samples, config.output_location);
    return 0;
}
