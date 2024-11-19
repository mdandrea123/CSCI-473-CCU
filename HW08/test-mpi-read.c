#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "MyMPI.h"
#include "utilities.h"

#define dtype double

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    if(argc != 4) {
        printf("usage: mpirun -np <p> %s <in data file> <out data file> <out data file from halo array> \n", argv[0]);
        MPI_Finalize();
        exit(1);
    }

    // First read in the matrix normally
    dtype** A; 
    int rows, cols;
    read_row_striped_matrix(argv[1], (void***)&A, MPI_DOUBLE, &rows, &cols, MPI_COMM_WORLD);

    // Print the matrix to verify it was read correctly
    print_row_striped_matrix((void**)A, MPI_DOUBLE, rows, cols, MPI_COMM_WORLD);

    // Write the matrix to a file
    write_row_striped_matrix(argv[2], (void**)A, MPI_DOUBLE, rows, cols, MPI_COMM_WORLD);

    // Now, let's make a new matrix in memory that is the 'halo' version of the matrix
    dtype** A2; 
    int rows2, cols2;

    // Read the matrix with halo rows
    read_row_striped_matrix_halo(argv[1], (void***)&A2, MPI_DOUBLE, &rows2, &cols2, MPI_COMM_WORLD);

    // Print the matrix with halo rows
    print_row_striped_matrix_halo((void**)A2, MPI_DOUBLE, rows2, cols2, MPI_COMM_WORLD);

    // Write the matrix, excluding halo rows
    write_row_striped_matrix_halo(argv[3], (void**)A2, MPI_DOUBLE, rows, cols, MPI_COMM_WORLD);

    my_free2d((double***)&A);
    my_free2d((double***)&A2);

    MPI_Finalize();
    return 0;
}