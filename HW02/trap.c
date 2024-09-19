#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <math.h>
#include <mpi.h>
#include "trap.h"

#define PI 3.14159265358979323846

void parse_input(int argc, char *argv[], int my_rank, double *a_p, double *b_p, int *n_p, int *f_p) {
    int opt;
    if (argc == 1 && my_rank == 0) {
        printf("Usage: mpirun -np <num_processes> ./mpi_trap -n <num divisions> -a <integrate from> -b <integrate to> -f <function to integrate>\n");
        printf("Functions: 1 = x, 2 = x^2, 3 = sin(x)\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    while ((opt = getopt(argc, argv, "n:a:b:f:")) != -1) {
        switch (opt) {
            case 'n':
                *n_p = atoi(optarg);
                break;
            case 'a':
                *a_p = atof(optarg);
                break;
            case 'b':
                *b_p = atof(optarg);
                break;
            case 'f':
                *f_p = atoi(optarg);
                break;
            default:
                if (my_rank == 0) {
                    printf("Invalid argument. Usage: mpirun -np <num_processes> ./mpi_trap -n <num divisions> -a <integrate from> -b <integrate to> -f <function to integrate>\n");
                }
                MPI_Abort(MPI_COMM_WORLD, 1);
        }
    }
}

double Trap(double left_endpt, double right_endpt, int trap_count, double base_len, int func_choice, int rank) {
    double estimate, x;
    int i;

    double (*func_ptr)(double);
    switch (func_choice) {
        case 1:
            func_ptr = f1;
            break;
        case 2:
            func_ptr = f2;
            break;
        case 3:
            func_ptr = f3;
            break;
        default:
            func_ptr = f1;  // Default to f1 if invalid function choice
    }
    printf("rank %d local a %.2f, local b %.2f, local n %d\n", rank, left_endpt, right_endpt, trap_count);

    estimate = (func_ptr(left_endpt) + func_ptr(right_endpt)) / 2.0;
    for (i = 1; i <= trap_count-1; i++) {
        x = left_endpt + i * base_len;
        estimate += func_ptr(x);
    }
    estimate = estimate * base_len;

    return estimate;
}

double f1(double x) {
    return x;
}

double f2(double x) {
    return x * x;
}

double f3(double x) {
    return sin(x * PI / 180.0);  // Convert degrees to radians
}
