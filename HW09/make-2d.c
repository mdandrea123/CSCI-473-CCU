#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "utilities.h"
#define dtype double

// Function to display the usage information for the program.
void usage(char **argv)
{
    printf("Usage: %s <num rows> <num cols> <outfile> \n", argv[0]);
}

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        usage(argv); // Display usage information if the number of command-line arguments is not 6.
        exit(1);
    }

    dtype **matrix = NULL;    // Declare a pointer to a 2D array of dtype.
    int rows = atoi(argv[1]); // Convert the first command-line argument to the number of rows.
    int cols = atoi(argv[2]); // Convert the second command-line argument to the number of columns.
    char *fname = argv[3];    // Store the third command-line argument as the output file name.

    // Allocate memory for a 2D array, initialize it with random values, write it to a file, and then free the memory.
    my_allocate2d(&matrix, rows, cols);
    initialize(matrix, rows, cols);
    write_to_file(matrix, rows, cols, fname);
    my_free2d(&matrix);

    return 0;
}
