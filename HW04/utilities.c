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

void write_row_striped_matrix(
    char* fname,       /* IN - output file name */
    void **A,          /* IN - 2D array */
    MPI_Datatype dtype,/* IN - Matrix element type */
    int rows,          /* IN - Matrix rows */
    int cols,          /* IN - Matrix cols */
    MPI_Comm comm)     /* IN - Communicator */
{
    int rank, size;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);

    FILE *f = NULL;
    if (rank == 0) {
        f = fopen(fname, "w");
        fcheck(f);  // Ensure the file was opened successfully

        // Write the matrix dimensions to the file
        fwrite(&rows, sizeof(int), 1, f);
        fwrite(&cols, sizeof(int), 1, f);
    }

    // Each process writes its portion of the matrix (its row stripe)
    int local_rows = BLOCK_SIZE(rank, size, rows);  // Calculate the number of rows for each process
    for (int i = 0; i < local_rows; i++) {
        if (rank == 0) {
            // Process 0 appends its rows after the header
            fwrite(A[i], sizeof(double), cols, f);
        } else {
            // Other processes send their rows to process 0
            MPI_Send(A[i], cols, dtype, 0, 0, comm);
        }
    }

    if (rank == 0) {
        // Process 0 receives the rows from other processes
        for (int p = 1; p < size; p++) {
            int local_rows_p = BLOCK_SIZE(p, size, rows);
            for (int i = 0; i < local_rows_p; i++) {
                double *buffer = (double*) malloc(cols * sizeof(double));
                MPI_Recv(buffer, cols, dtype, p, 0, comm, MPI_STATUS_IGNORE);
                fwrite(buffer, sizeof(double), cols, f);
                free(buffer);
            }
        }
        fclose(f);
    }
}


