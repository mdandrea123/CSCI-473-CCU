#include <stdio.h>
#include <stdlib.h>
#include "utilities.h"
#include "timer.h"

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <rows> <cols> <output_file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int rows = atoi(argv[1]);
    int cols = atoi(argv[2]);
    char *output_file = argv[3];

    if (rows <= 0 || cols <= 0) {
        fprintf(stderr, "Error: rows and columns must be positive integers\n");
        exit(EXIT_FAILURE);
    }

    // Allocate 2D array
    double **data = allocate_2d_array(rows, cols);

    // Initialize the array
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (j == 0 || j == cols - 1) {
                data[i][j] = 1.0;  // Left and right walls
            } else if (i == 0 || i == rows - 1) {
                data[i][j] = 0.0;  // Top and bottom walls
            } else {
                data[i][j] = 0.0;  // Interior points
            }
        }
    }

    // Write to binary file
    FILE *fp = fopen(output_file, "wb");
    if (fp == NULL) {
        fprintf(stderr, "Error opening file for writing\n");
        exit(EXIT_FAILURE);
    }

    // Write metadata
    fwrite(&rows, sizeof(int), 1, fp);
    fwrite(&cols, sizeof(int), 1, fp);

    // Write data
    for (int i = 0; i < rows; i++) {
        fwrite(data[i], sizeof(double), cols, fp);
    }

    fclose(fp);

    // Free allocated memory
    free_2d_array(data, rows);

    printf("Initial conditions written to %s\n", output_file);
    return 0;
}