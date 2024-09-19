#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "trap.h"

int main(int argc, char* argv[]) {
    int my_rank, comm_sz, n, local_n, func_choice;
    double a, b, h, local_a, local_b;
    double local_int, total_int;

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    parse_input(argc, argv, my_rank, &a, &b, &n, &func_choice);

    h = (b - a) / n;

    // Using Quinn's macros to calculate local_n and distribute the workload
    local_n = BLOCK_SIZE(my_rank, comm_sz, n);
    local_a = a + BLOCK_LOW(my_rank, comm_sz, n) * h;
    local_b = local_a + local_n * h;

    local_int = Trap(local_a, local_b, local_n, h, func_choice, my_rank);

    MPI_Reduce(&local_int, &total_int, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (my_rank == 0) {
        printf("With n = %d trapezoids, our estimate\n", n);
        printf("of the integral from %f to %f = %.2f\n", a, b, total_int);
    }

    MPI_Finalize();
    return 0;
}
