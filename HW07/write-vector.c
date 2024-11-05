#include <stdio.h>
#include <stdlib.h>
#include "functions.h"

int main(int argc, char **argv){
    int size = atoi(argv[1]);
    char *output_file = argv[2];

    double *vector = (double *)malloc(size * sizeof(double));
    init_vector(vector, size);

    // Write to binary file
    FILE *fp = fopen(output_file, "wb");

    if (fp == NULL) {
        fprintf(stderr, "Error opening file for writing\n");
        exit(EXIT_FAILURE);
    }

    // Write metadata
    fwrite(&size, sizeof(int), 1, fp);

    // Write data
    fwrite(vector, sizeof(double), size, fp);

    fclose(fp);

    // Print vector to screen
    for (int i = 0; i < size; i++)
    {
        printf("%f ", vector[i]);
        printf("\n");
    }

    // Free allocated memory
    free(vector);
}