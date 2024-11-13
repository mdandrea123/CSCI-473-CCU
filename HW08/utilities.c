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

int get_local_rows_with_halo(int rank, int size, int m) {
    int local_rows = BLOCK_SIZE(rank, size, m);
    
    if (size == 1) {
        // For single process, no halo rows needed
        return local_rows;
    } else if (rank == 0) {
        return local_rows + 1;  // Only need bottom halo
    } else if (rank == size - 1) {
        return local_rows + 1;  // Only need top halo
    } else {
        return local_rows + 2;  // Need both halos
    }
}

void read_row_striped_matrix_halo(
    char *s,            /* IN - File name */
    void ***subs,       /* OUT - 2D submatrix indices */
    MPI_Datatype dtype, /* IN - Matrix element type */
    int *m,             /* OUT - Matrix rows */
    int *n,             /* OUT - Matrix cols */
    MPI_Comm comm)      /* IN - Communicator */
{
    int rank, size;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);
    
    // Process size-1 reads the dimensions
    if (rank == (size-1)) {
        FILE *f = fopen(s, "r");
        if (f == NULL) *m = 0;
        else {
            fread(m, sizeof(int), 1, f);
            fread(n, sizeof(int), 1, f);
            fclose(f);
        }
    }
    
    // Broadcast dimensions to all processes
    MPI_Bcast(m, 1, MPI_INT, size-1, comm);
    if (!(*m)) MPI_Abort(comm, OPEN_FILE_ERROR);
    MPI_Bcast(n, 1, MPI_INT, size-1, comm);
    
    // Calculate local rows including halos
    int local_rows = get_local_rows_with_halo(rank, size, *m);
    
    // Allocate space for the submatrix including halos
    my_allocate2d((double ***)subs, local_rows, *n);
    
    // Initialize halo rows to -1
    double **matrix = (double **)*subs;
    if (size > 1) {  // Only add halos if we have multiple processes
        if (rank != 0) {  // Top halo for all except process 0
            for (int j = 0; j < *n; j++) {
                matrix[0][j] = -1.0;
            }
        }
        if (rank != size-1) {  // Bottom halo for all except last process
            for (int j = 0; j < *n; j++) {
                matrix[local_rows-1][j] = -1.0;
            }
        }
    }
    
    // Read the actual data from file
    if (rank == (size-1)) {
        FILE *f = fopen(s, "r");
        fseek(f, 2 * sizeof(int), SEEK_SET);  // Skip header
        
        // Read and distribute data to other processes
        for (int i = 0; i < size-1; i++) {
            int rows = BLOCK_SIZE(i, size, *m);
            double *buffer = malloc(rows * (*n) * sizeof(double));
            fread(buffer, sizeof(double), rows * (*n), f);
            MPI_Send(buffer, rows * (*n), MPI_DOUBLE, i, DATA_MSG, comm);
            free(buffer);
        }
        
        // Read own portion
        int start_row = (size > 1 && rank != 0) ? 1 : 0;  // Skip halo row if not process 0 and multiple processes
        fread(matrix[start_row], sizeof(double), BLOCK_SIZE(rank, size, *m) * (*n), f);
        fclose(f);
    } else {
        // Receive data from last process
        int start_row = (rank == 0) ? 0 : 1;  // Skip halo row if not process 0
        MPI_Recv(matrix[start_row], BLOCK_SIZE(rank, size, *m) * (*n), 
                MPI_DOUBLE, size-1, DATA_MSG, comm, MPI_STATUS_IGNORE);
    }
}

void print_row_striped_matrix_halo(
    void **a,            /* IN - 2D array */
    MPI_Datatype dtype,  /* IN - Matrix element type */
    int m,               /* IN - Matrix rows */
    int n,               /* IN - Matrix cols */
    MPI_Comm comm)       /* IN - Communicator */
{
    int rank, size;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);
    
    // Process 0 prints its portion first
    if (rank == 0) {
        int local_rows = get_local_rows_with_halo(0, size, m);
        print_submatrix(a, dtype, local_rows, n);
        
        // Receive and print from other processes
        for (int i = 1; i < size; i++) {
            int rows = get_local_rows_with_halo(i, size, m);
            double **buffer;
            my_allocate2d(&buffer, rows, n);
            
            MPI_Send(&rank, 1, MPI_INT, i, PROMPT_MSG, comm);  // Send prompt
            MPI_Recv(buffer[0], rows * n, dtype, i, RESPONSE_MSG, comm, MPI_STATUS_IGNORE);
            
            print_submatrix((void **)buffer, dtype, rows, n);
            my_free2d(&buffer);
        }
    } else {
        // Other processes wait for their turn and send their data
        int prompt;
        int local_rows = get_local_rows_with_halo(rank, size, m);
        MPI_Recv(&prompt, 1, MPI_INT, 0, PROMPT_MSG, comm, MPI_STATUS_IGNORE);
        MPI_Send(a[0], local_rows * n, dtype, 0, RESPONSE_MSG, comm);
    }
}

void write_row_striped_matrix_halo(
    char *s,            /* IN - File name */
    void **a,           /* IN - 2D array */
    MPI_Datatype dtype, /* IN - Matrix element type */
    int m,              /* IN - Original matrix rows */
    int n,              /* IN - Matrix cols */
    MPI_Comm comm)      /* IN - Communicator */
{
    int rank, size;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);
    
    // Process 0 creates file and writes header
    if (rank == 0) {
        FILE *f = fopen(s, "w");
        fcheck(f);
        fwrite(&m, sizeof(int), 1, f);
        fwrite(&n, sizeof(int), 1, f);
        
        // Write process 0's data (excluding bottom halo)
        int local_rows = BLOCK_SIZE(0, size, m);
        fwrite(a[0], sizeof(double), local_rows * n, f);
        
        // Receive and write data from other processes
        for (int i = 1; i < size; i++) {
            int proc_rows = BLOCK_SIZE(i, size, m);
            double *buffer = malloc(proc_rows * n * sizeof(double));
            
            MPI_Send(&rank, 1, MPI_INT, i, PROMPT_MSG, comm);
            MPI_Recv(buffer, proc_rows * n, dtype, i, RESPONSE_MSG, comm, MPI_STATUS_IGNORE);
            
            fwrite(buffer, sizeof(double), proc_rows * n, f);
            free(buffer);
        }
        fclose(f);
    } else {
        // Other processes send their data (excluding halos)
        int prompt;
        int local_rows = BLOCK_SIZE(rank, size, m);
        MPI_Recv(&prompt, 1, MPI_INT, 0, PROMPT_MSG, comm, MPI_STATUS_IGNORE);
        // Send from row 1 (after top halo) for the actual number of rows
        MPI_Send(a[1], local_rows * n, dtype, 0, RESPONSE_MSG, comm);
    }
}
