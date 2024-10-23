#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utilities.h"
#include "timer.h"

int main(int argc, char *argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Usage: %s <num_iterations> <input_file> <output_file> <all_iterations>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int iterations = atoi(argv[1]);
    char *input_file = argv[2];
    char *output_file = argv[3];
    char *all_iterations_file = argv[4];

    double start, finish, elapsed;

    int rows, cols;

    if (iterations <= 0) {
        fprintf(stderr, "Error: number of iterations must be positive\n");
        exit(EXIT_FAILURE);
    }

    GET_TIME(start);

    // Open the input file
    FILE *fp = fopen(input_file, "rb");
    if (fp == NULL) {
        fprintf(stderr, "Error opening input file for reading\n");
        exit(EXIT_FAILURE);
    }

    // Read metadata
    fread(&rows, sizeof(int), 1, fp);
    fread(&cols, sizeof(int), 1, fp);

    // Allocate 2D arrays
    double **data = allocate_2d_array(rows, cols);
    double **temp = allocate_2d_array(rows, cols);

    // Read initial data
    for (int i = 0; i < rows; i++) {
        fread(data[i], sizeof(double), cols, fp);
    }
    fclose(fp);

    // Open all iterations file
    FILE *all_fp = fopen(all_iterations_file, "wb");
    if (all_fp == NULL) {
        fprintf(stderr, "Error opening all iterations file for writing\n");
        exit(EXIT_FAILURE);
    }

    // Write metadata to all iterations file
    fwrite(&rows, sizeof(int), 1, all_fp);
    fwrite(&cols, sizeof(int), 1, all_fp);

    // Write initial state to all iterations file
    for (int i = 0; i < rows; i++) {
        fwrite(data[i], sizeof(double), cols, all_fp);
    }

    // Perform stencil operations
    for (int iter = 0; iter < iterations; iter++) {
        apply_stencil(data, temp, rows, cols);
        
        // Copy boundary conditions
        for (int i = 0; i < rows; i++) {
            temp[i][0] = data[i][0];
            temp[i][cols-1] = data[i][cols-1];
        }
        for (int j = 0; j < cols; j++) {
            temp[0][j] = data[0][j];
            temp[rows-1][j] = data[rows-1][j];
        }

        // Swap data and temp
        double **swap = data;
        data = temp;
        temp = swap;

        // Write current state to all iterations file
        for (int i = 0; i < rows; i++) {
            fwrite(data[i], sizeof(double), cols, all_fp);
        }
    }

    fclose(all_fp);

    // Write final state to output file
    FILE *out_fp = fopen(output_file, "wb");
    if (out_fp == NULL) {
        fprintf(stderr, "Error opening output file for writing\n");
        exit(EXIT_FAILURE);
    }

    // Write metadata
    fwrite(&rows, sizeof(int), 1, out_fp);
    fwrite(&cols, sizeof(int), 1, out_fp);

    // Write final data
    for (int i = 0; i < rows; i++) {
        fwrite(data[i], sizeof(double), cols, out_fp);
    }

    fclose(out_fp);

    // Free allocated memory
    free_2d_array(data, rows);
    free_2d_array(temp, rows);

    GET_TIME(finish);
    elapsed = finish - start;

    printf("Stencil operation completed. Final state written to %s\n", output_file);
    printf("All iterations written to %s\n", all_iterations_file);
    printf("Elapsed time = %e seconds\n", elapsed);

    return 0;
}