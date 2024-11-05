#include <stdio.h>
#include <stdlib.h>
#include "functions.h"

void malloc2D(double ***a, int jmax, int imax);


int main(int argc, char **argv){
    int rows = atoi(argv[1]);
    int cols = atoi(argv[2]);
    char *output_file = argv[3];

    double **data;
    malloc2D(&data, rows, cols); //From previous HW06
    double count = 1.0;

    // Initialize the array
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            data[i][j] = count;
            count++;
        }
    }

    // Write to binary file
    FILE *fp = fopen(output_file, "wb");

    if (fp == NULL) {
        fprintf(stderr, "Error opening file for writing\n");
        exit(EXIT_FAILURE);
    }

    // Write metadata
    fwrite(&rows, sizeof(int), 1, fp);
    fwrite(&cols, sizeof(int), 1, fp);

    // Write data
    fwrite(data[0], sizeof(double), rows*cols, fp);

    //print the data
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%f ", data[i][j]);
        }
        printf("\n");
    }

    fclose(fp);

    // Free allocated memory
    free(data);
}
