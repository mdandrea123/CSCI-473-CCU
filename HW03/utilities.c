#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "MyMPI.h"
#include "utilities.h"

// Function to check if a file pointer is valid (not NULL).
// If it's not valid, it prints an error message and exits the program.
void fcheck(FILE *fp)
{
    if (!fp)
    {
        perror("Error opening file: ");
        exit(1);
    }
}

// Function to allocate a 2D integer array dynamically.
// It takes a pointer to the 2d array, the number of rows, and the number of columns as input and allocates memory for the 2D array.
void my_allocate2d(double ***A, int rows, int cols)
{
    size_t header = rows * sizeof(double *);
    size_t data = rows * cols * sizeof(double);

    *A = (double **)malloc(header + data);
    double *array_data = (double *)((*A) + rows);

    for (int i = 0; i < rows; i++)
    {
        (*A)[i] = array_data + i * cols;
    }
}



// Function to free the memory allocated for a 2D integer array.
// It takes a pointer to a pointer to int as input and sets it to NULL after freeing the memory.
void my_free2d(double ***A)
{
    free(*A);
    *A = NULL;
}

// Function to initialize a 2D array with ascending values starting at 1.
// It takes a pointer to a 2D array, the number of rows and columns as input.
void initialize(double **A, int rows, int cols) {
    double count = 1.0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            A[i][j] = count;
            count++;
        }
    }
}

// Function to write a 2D array to a binary file.
// It takes a pointer to a 2D array, the number of rows, columns, and a file name as input.
void write_to_file(double **A, int rows, int cols, char *fname)
{
    FILE *f = fopen(fname, "w");
    fcheck(f);

    fwrite(&rows, sizeof(int), 1, f);
    fwrite(&cols, sizeof(int), 1, f);

    fwrite(A[0], sizeof(double), rows * cols, f);

    fclose(f);
}
