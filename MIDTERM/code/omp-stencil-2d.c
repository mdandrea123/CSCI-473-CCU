#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include "utilities.h"
#include "timer.h"

int main(int argc, char *argv[]) {
    if (argc < 5 || argc > 7) {
        fprintf(stderr, "Usage: %s <num_iterations> <input_file> <output_file> <debug_level> [num_threads] [all_iterations_file]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int iterations = atoi(argv[1]);
    char *input_file = argv[2];
    char *output_file = argv[3];
    int debug_level = atoi(argv[4]);
    int num_threads = (argc > 5) ? atoi(argv[5]) : 1;
    char *all_iterations_file = (argc > 6) ? argv[6] : NULL;
    
    double start_total, finish_total, elapsed_total;
    double start_comp, finish_comp, elapsed_comp = 0.0;
    int rows, cols;
    
    GET_TIME(start_total);

    // Set number of threads
    omp_set_num_threads(num_threads);

    // Read input data
    FILE *fp = fopen(input_file, "rb");
    if (!fp) {
        fprintf(stderr, "Error opening input file\n");
        exit(EXIT_FAILURE);
    }

    fread(&rows, sizeof(int), 1, fp);
    fread(&cols, sizeof(int), 1, fp);

    if (debug_level >= 1) {
        printf("Reading input file: %s\n", input_file);
        printf("Matrix dimensions: %d rows x %d columns\n", rows, cols);
        printf("Number of iterations: %d\n", iterations);
        printf("Number of threads: %d\n", num_threads);
        if (all_iterations_file) {
            printf("Writing all iterations to: %s\n", all_iterations_file);
        }
        printf("\n");
    }

    double **data = allocate_2d_array(rows, cols);
    double **temp = allocate_2d_array(rows, cols);

    for (int i = 0; i < rows; i++) {
        fread(data[i], sizeof(double), cols, fp);
    }
    fclose(fp);

    // Initialize all iterations file if specified
    FILE *all_fp = NULL;
    if (all_iterations_file) {
        all_fp = fopen(all_iterations_file, "wb");
        if (!all_fp) {
            fprintf(stderr, "Error opening all iterations file\n");
            exit(EXIT_FAILURE);
        }
        fwrite(&rows, sizeof(int), 1, all_fp);
        fwrite(&cols, sizeof(int), 1, all_fp);
        
        // Write initial state
        for (int i = 0; i < rows; i++) {
            fwrite(data[i], sizeof(double), cols, all_fp);
        }
    }

    if (debug_level >= 1) {
        long data_size = rows * cols * sizeof(double) + 2 * sizeof(int);
        printf("Input file size: %ld bytes\n", data_size);
        printf("Starting computation...\n\n");
    }

    // Main iteration loop
    for (int iter = 0; iter < iterations; iter++) {
        GET_TIME(start_comp);
        
        // Parallel stencil computation
        #pragma omp parallel for
        for (int i = 1; i < rows - 1; i++) {
            for (int j = 1; j < cols - 1; j++) {
                temp[i][j] = (data[i-1][j-1] + data[i-1][j] + data[i-1][j+1] +
                             data[i][j+1]   + data[i+1][j+1]   + data[i+1][j] +
                             data[i+1][j-1] + data[i][j-1] + data[i][j]) / 9.0;
            }
        }

        // Copy boundary conditions
        #pragma omp parallel for
        for (int i = 0; i < rows; i++) {
            temp[i][0] = data[i][0];
            temp[i][cols-1] = data[i][cols-1];
        }

        #pragma omp parallel for
        for (int j = 0; j < cols; j++) {
            temp[0][j] = data[0][j];
            temp[rows-1][j] = data[rows-1][j];
        }

        // Swap arrays
        double **swap = data;
        data = temp;
        temp = swap;

        GET_TIME(finish_comp);
        elapsed_comp += (finish_comp - start_comp);

        // Write current state to all iterations file if specified
        if (all_fp) {
            for (int i = 0; i < rows; i++) {
                fwrite(data[i], sizeof(double), cols, all_fp);
            }
        }

        // Print debug information if requested
        if (debug_level == 2) {
            printf("\nAfter iteration %d:\n", iter + 1);
            print_2d_array(data, rows, cols);
        }
    }

    // Write final state
    FILE *out_fp = fopen(output_file, "wb");
    if (!out_fp) {
        fprintf(stderr, "Error opening output file\n");
        exit(EXIT_FAILURE);
    }

    fwrite(&rows, sizeof(int), 1, out_fp);
    fwrite(&cols, sizeof(int), 1, out_fp);
    for (int i = 0; i < rows; i++) {
        fwrite(data[i], sizeof(double), cols, out_fp);
    }
    fclose(out_fp);
    if (all_fp) fclose(all_fp);

    if (debug_level >= 1) {
        printf("Computation completed.\n");
        printf("Final state written to: %s\n\n", output_file);
    }

    // Clean up
    free_2d_array(data, rows);
    free_2d_array(temp, rows);

    GET_TIME(finish_total);
    elapsed_total = finish_total - start_total;

    if (debug_level >= 0) {
        printf("Time overall = %e seconds\n", elapsed_total);
        printf("Time computation = %e seconds\n", elapsed_comp);
        printf("Time other = %e seconds\n", elapsed_total - elapsed_comp);
    }

    return 0;
}