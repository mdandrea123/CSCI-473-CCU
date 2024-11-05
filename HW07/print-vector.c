#include <stdio.h>
#include <stdlib.h>
#include "functions.h"

int main(int argc, char **argv){
    char *input_file = argv[1];
    int size;
    double *vector;

    // Read vector from binary file
    FILE *fp = fopen(input_file, "rb");

    if (fp == NULL) {
        fprintf(stderr, "Error opening file for reading\n");
        exit(EXIT_FAILURE);
    }

    // Read metadata
    fread(&size, sizeof(int), 1, fp);

    // Allocate memory for the vector
    vector = (double *)malloc(size * sizeof(double));

    // Read data
    fread(vector, sizeof(double), size, fp);

    fclose(fp);

    // Print vector to screen
    print_vector(vector, size);

    // Free allocated memory
    free(vector);

    return 0;
}
