#include <stdio.h>
#include <stdlib.h>

void malloc2D(double ***a, int jmax, int imax)
{
    // first allocate a block of memory for the row pointers and the 2D array
    double **x = (double **)malloc(jmax*sizeof(double *) + jmax*imax*sizeof(double));
    
    // Now assign the start of the block of memory for the 2D array after the row pointers
    x[0] = (double *)x + jmax;
    
    // Last, assign the memory location to point to for each row pointer
    for (int j = 1; j < jmax; j++) {
        x[j] = x[j-1] + imax;
    }
    
    *a = x;
}

void init_vector(double *vector, int size)
{
    double count = 1.0;
    for (int i = 0; i < size; i++)
    {
        vector[i] = count;
        count++;
    }
}

void print_vector(double *vector, int size)
{
    for (int i = 0; i < size; i++)
    {
        printf("%f ", vector[i]);
        printf("\n");
    }
    printf("\n");
}