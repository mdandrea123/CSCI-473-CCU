#include <stdio.h>
#include <mpi.h>
#include "functions.h"
#include <math.h>

void global_sum(double* result, int rank, int size, double my_value) {
    int partner, phase, num_phases;
    double received_value, sum = my_value;  // Initialize sum with my_value

    // Calculate the number of phases (log2(size))
    num_phases = (int) log2(size);

    for (phase = 0; phase < num_phases; phase++) {
        // Determine partner for this phase
        partner = rank ^ (1 << phase);

        if (rank < partner) {
            // Receive from partner
            MPI_Recv(&received_value, 1, MPI_DOUBLE, partner, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf(" Phase %d - P %d (%02d) receiving from P %d (%02d), val %.1f\n", phase, rank, rank, partner, partner, received_value);
            sum += received_value;  // Accumulate the received value
            MPI_Ssend(&my_value, 1, MPI_DOUBLE, partner, 0, MPI_COMM_WORLD);
            printf(" Phase %d - P %d (%02d) sending   to   P %d (%02d), val %.1f\n", phase, rank, rank, partner, partner, my_value);
        } else {
            MPI_Ssend(&my_value, 1, MPI_DOUBLE, partner, 0, MPI_COMM_WORLD);
            printf(" Phase %d - P %d (%02d) sending   to   P %d (%02d), val %.1f\n", phase, rank, rank, partner, partner, my_value);
            MPI_Recv(&received_value, 1, MPI_DOUBLE, partner, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf(" Phase %d - P %d (%02d) receiving from P %d (%02d), val %.1f\n", phase, rank, rank, partner, partner, received_value);
            sum += received_value;  // Accumulate the received value
        }

        // After each phase, update my_value with the accumulated sum so far
        my_value = sum;
    }

    // Store the final result in the provided pointer
    *result = sum;
}
