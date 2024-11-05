#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "functions.h"

// Function prototypes
void read_matrix(const char *filename, double ***matrix, int *rows, int *cols);
void read_vector(const char *filename, double **vector, int *size);
void write_vector(const char *filename, double *vector, int size);
void matrix_vector_multiply(double **matrix, double *vector, double *result, int rows, int cols, int num_threads);

int main(int argc, char *argv[])
{
    if (argc != 5)
    {
        fprintf(stderr, "Usage: %s <num threads> <matrix A file> <vector X file> <output Y file>\n", argv[0]);
        return 1;
    }

    int num_threads = atoi(argv[1]);
    const char *matrix_file = argv[2];
    const char *vector_file = argv[3];
    const char *output_file = argv[4];

    double **A; // Matrix A
    double *X;  // Vector X
    double *Y;  // Output vector Y
    int rows, cols, vector_size;

    // Read matrix and vector from files
    read_matrix(matrix_file, &A, &rows, &cols);
    read_vector(vector_file, &X, &vector_size);

    // Ensure the matrix and vector dimensions are compatible
    if (cols != vector_size)
    {
        fprintf(stderr, "Error: Matrix columns (%d) and vector size (%d) do not match!\n", cols, vector_size);
        return 1;
    }

    // Allocate memory for the result vector
    Y = (double *)malloc(rows * sizeof(double));

    // Perform matrix-vector multiplication
    matrix_vector_multiply(A, X, Y, rows, cols, num_threads);

    // Write the result vector to the output file
    write_vector(output_file, Y, rows);

    // Free dynamically allocated memory
    for (int i = 0; i < rows; i++)
    {
        free(A[i]);
    }
    free(A);
    free(X);
    free(Y);

    return 0;
}

// Function to read matrix from a binary file
void read_matrix(const char *filename, double ***matrix, int *rows, int *cols) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Error opening matrix file");
        exit(EXIT_FAILURE);
    }

    // Read the number of rows and columns
    fread(rows, sizeof(int), 1, file);
    fread(cols, sizeof(int), 1, file);

    // Allocate memory for the matrix
    *matrix = (double **)malloc(*rows * sizeof(double *));
    for (int i = 0; i < *rows; i++) {
        (*matrix)[i] = (double *)malloc(*cols * sizeof(double));
    }

    // Read the matrix data
    fread(&(*matrix)[0][0], sizeof(double), (*rows) * (*cols), file);

    fclose(file);
}


// Function to read vector from a binary file
void read_vector(const char *filename, double **vector, int *size) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Error opening vector file");
        exit(EXIT_FAILURE);
    }

    // Read the size of the vector
    fread(size, sizeof(int), 1, file);
    
    // Allocate memory for the vector
    *vector = (double *)malloc(*size * sizeof(double));

    // Read the vector data
    fread(*vector, sizeof(double), *size, file);

    fclose(file);
}


// Function to write vector to a binary file
void write_vector(const char *filename, double *vector, int size)
{
    FILE *file = fopen(filename, "wb");
    if (!file)
    {
        perror("Error opening output file");
        exit(EXIT_FAILURE);
    }

    // Write the result vector size followed by the vector data
    fwrite(&size, sizeof(int), 1, file);
    fwrite(vector, sizeof(double), size, file);

    fclose(file);
}

// Function to perform matrix-vector multiplication
void matrix_vector_multiply(double **matrix, double *vector, double *result, int rows, int cols, int num_threads)
{
#pragma omp parallel for num_threads(num_threads)
    for (int i = 0; i < rows; i++)
    {
        result[i] = 0.0;
        for (int j = 0; j < cols; j++)
        {
            result[i] += matrix[i][j] * vector[j];
        }
    }
}
