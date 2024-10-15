#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

void parse(int argc, char *argv[], double *A, double *C, double *H, int *n){
    int opt;
    while ((opt = getopt(argc, argv, "A:C:H:n:")) != -1) {
        switch (opt) {
            case 'A': *A = atof(optarg); break;
            case 'C': *C = atof(optarg); break;
            case 'H': *H = atof(optarg); break;
            case 'n': *n = atoi(optarg); break;
            default:
                fprintf(stderr, "Usage: %s -A <value> -C <value> -H <value> -n <value>\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    if (*A == 0.0 || *C == 0.0 || *H == 0.0 || *n == 0) {
        fprintf(stderr, "All arguments (-A, -C, -H, -n) are required.\n");
        exit(EXIT_FAILURE);
    }
}