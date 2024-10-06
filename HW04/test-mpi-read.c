#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "utilities.h"
#include "MyMPI.h"

#define dtype double

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (argc != 3) {
        printf("usage: mpirun -np <p> %s <in data file> <outfile>\n", argv[0]);
        MPI_Finalize();
        exit(1);
    }

    dtype** A; 
    int rows, cols; // Note, only the subscripts are being passed, not the data/storage
    char *fname = argv[2];    // Store the third command-line argument as the output file name.

    
    
    read_row_striped_matrix(argv[1], (void***)&A, MPI_DOUBLE, &rows, &cols, MPI_COMM_WORLD);

    if(size > rows){
        if (rank == 0) {
            printf("Number of processes must be less than or equal to the number of rows\n");
        }
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }
    
    print_row_striped_matrix((void**)A, MPI_DOUBLE, rows, cols, MPI_COMM_WORLD);
    write_row_striped_matrix(fname, (void**)A, MPI_DOUBLE, rows, cols, MPI_COMM_WORLD);
    
    my_free2d((double ***)&A); // Cast to the expected type


    MPI_Finalize();
    return 0;
}