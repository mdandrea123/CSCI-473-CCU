#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "utilities.h"
#include "MyMPI.h"

void usage(char **argv) {
    printf("usage: %s <input_file>\n", argv[0]);
}

int main(int argc, char **argv) {
    if (argc != 2) {
        usage(argv);
        exit(1);
    }

    // Variables to store the matrix dimensions and data
    double **matrix;
    int rows, cols;
    
    // Open the input file
    FILE *fp = fopen(argv[1], "r");
    if (!fp) {
        perror("Error opening file");
        exit(1);
    }
    
    // Read the dimensions
    printf("reading in file: %s\n", argv[1]);
    fread(&rows, sizeof(int), 1, fp);
    fread(&cols, sizeof(int), 1, fp);
    
    // Allocate memory for the matrix
    my_allocate2d(&matrix, rows, cols);
    
    // Read the matrix data
    fread(matrix[0], sizeof(double), rows * cols, fp);
    
    // Print the matrix
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%5.2f ", matrix[i][j]);
        }
        printf("\n");
    }
    
    // Clean up
    fclose(fp);
    my_free2d(&matrix);
    
    return 0;
}