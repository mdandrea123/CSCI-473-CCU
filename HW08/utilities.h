#ifndef UTILITIES_H
#define UTILITIES_H

// Function prototypes
void fcheck(FILE *fp);
void my_allocate2d(double ***A, int rows, int cols);
void my_free2d(double ***A);
void initialize(double **A, int rows, int cols);
void write_to_file(double **A, int rows, int cols, char *fname);
void write_row_striped_matrix(char *outFile, void **a, MPI_Datatype dtype, int m, int n, MPI_Comm comm);

// Print function from MyMPI
void print_submatrix(void **a, MPI_Datatype dtype, int rows, int cols);

// New halo functions
void read_row_striped_matrix_halo(char *s, void ***subs, MPI_Datatype dtype, int *m, int *n, MPI_Comm comm);
void print_row_striped_matrix_halo(void **a, MPI_Datatype dtype, int m, int n, MPI_Comm comm);
void write_row_striped_matrix_halo(char *s, void **a, MPI_Datatype dtype, int m, int n, MPI_Comm comm);

#endif