#include <stdio.h>
#include <stdlib.h>
#include "utilities.h"
#include "timer.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *input_file = argv[1];
    int rows, cols;

    double start, finish, elapsed;

    GET_TIME(start);

    // Open the input file
    FILE *fp = fopen(input_file, "rb");
    if (fp == NULL) {
        fprintf(stderr, "Error opening file for reading\n");
        exit(EXIT_FAILURE);
    }

    // Read metadata
    fread(&rows, sizeof(int), 1, fp);
    fread(&cols, sizeof(int), 1, fp);

    // Allocate 2D array
    double **data = allocate_2d_array(rows, cols);

    // Read data
    for (int i = 0; i < rows; i++) {
        fread(data[i], sizeof(double), cols, fp);
    }

    fclose(fp);

    GET_TIME(finish);
    elapsed = finish - start;

    // Print the data
    printf("Data from file %s (%d rows x %d columns):\n\n", input_file, rows, cols);
    print_2d_array(data, rows, cols);
    
    printf("\nElapsed time = %e seconds\n", elapsed);

    // Free allocated memory
    free_2d_array(data, rows);

    return 0;
}