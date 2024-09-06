#include <stdio.h>
#include <mpi.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "functions.h"

// Helper function to convert an integer to its binary representation
void int_to_binary(int n, char *binary_str, int bits)
{
    binary_str[bits] = '\0'; // Null-terminate the string
    for (int i = bits - 1; i >= 0; i--)
    {
        binary_str[i] = (n & 1) ? '1' : '0';
        n >>= 1;
    }
}
int is_power_of_2(int n)
{
    return (n & (n - 1)) == 0;
}

void global_sum(double *result, int rank, int size, double my_value)
{
    if (!is_power_of_2(size))
    {
        if (rank == 0)
        {
            fprintf(stderr, "Error: global_sum() requires a power of 2 number of processes\n");
        }
        MPI_Finalize();
        exit(1);
    }
    int partner, phase, num_phases;
    double received_value, sum = my_value; // Initialize sum with my_value
    char rank_binary[MAX_BINARY_LEN], partner_binary[MAX_BINARY_LEN];

    // Calculate the number of phases (log2(size))
    num_phases = (int)log2(size);

    for (phase = 0; phase < num_phases; phase++)
    {
        // Determine partner for this phase
        partner = rank ^ (1 << phase);

        // Convert rank and partner to binary strings
        int_to_binary(rank, rank_binary, num_phases);
        int_to_binary(partner, partner_binary, num_phases);

        if (rank < partner)
        {
            // Receive from partner
            MPI_Recv(&received_value, 1, MPI_DOUBLE, partner, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf(" Phase %d - P %d (%s) receiving from P %d (%s), val %.1f\n", phase, rank, rank_binary, partner, partner_binary, received_value);
            sum += received_value; // Accumulate the received value
            MPI_Ssend(&my_value, 1, MPI_DOUBLE, partner, 0, MPI_COMM_WORLD);
            printf(" Phase %d - P %d (%s) sending   to   P %d (%s), val %.1f\n", phase, rank, rank_binary, partner, partner_binary, my_value);
        }
        else
        {
            MPI_Ssend(&my_value, 1, MPI_DOUBLE, partner, 0, MPI_COMM_WORLD);
            printf(" Phase %d - P %d (%s) sending   to   P %d (%s), val %.1f\n", phase, rank, rank_binary, partner, partner_binary, my_value);
            MPI_Recv(&received_value, 1, MPI_DOUBLE, partner, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf(" Phase %d - P %d (%s) receiving from P %d (%s), val %.1f\n", phase, rank, rank_binary, partner, partner_binary, received_value);
            sum += received_value; // Accumulate the received value
        }

        // After each phase, update my_value with the accumulated sum so far
        my_value = sum;
    }

    // Store the final result in the provided pointer
    *result = sum;
}
