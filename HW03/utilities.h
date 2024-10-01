#ifndef UTILITIES_H
#define UTILITIES_H

// Function prototypes
void fcheck(FILE *fp);
void my_allocate2d(double ***A, int rows, int cols);
void my_free2d(double ***A);
void initialize(double **A, int rows, int cols);
void write_to_file(double **A, int rows, int cols, char *fname);
#endif
