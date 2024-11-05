// utilities.h

#ifndef UTILITIES_H
#define UTILITIES_H

double **allocate_2d_array(int rows, int cols);
void free_2d_array(double **arr, int rows);
void print_2d_array(double **data, int rows, int cols);
void apply_stencil(double **in, double **out, int rows, int cols);

#endif // UTILITIES_H