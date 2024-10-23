// utilities.c

#include <stdlib.h>
#include <stdio.h>
#include "utilities.h"

double **allocate_2d_array(int rows, int cols) {
    double **arr = (double **)malloc(rows * sizeof(double *));
    for (int i = 0; i < rows; i++) {
        arr[i] = (double *)malloc(cols * sizeof(double));
    }
    return arr;
}

void free_2d_array(double **arr, int rows) {
    for (int i = 0; i < rows; i++) {
        free(arr[i]);
    }
    free(arr);
}

void print_2d_array(double **data, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%6.2f ", data[i][j]);
        }
        printf("\n");
    }
}

void apply_stencil(double **in, double **out, int rows, int cols) {
    for (int i = 1; i < rows - 1; i++) {
        for (int j = 1; j < cols - 1; j++) {
            out[i][j] = (in[i-1][j-1] + in[i-1][j] + in[i-1][j+1] +
                         in[i][j+1]   + in[i+1][j+1]   + in[i+1][j] +
                         in[i+1][j-1] + in[i][j-1] + in[i][j]) / 9.0;
        }
    }
}