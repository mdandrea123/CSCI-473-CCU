#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <unistd.h>

int is_power_of_two(int n)
{
    return (n & (n - 1)) == 0;
}

void global_sum(double *result, int rank, int size, double my_value)
{
    if (!is_power_of_two(size))
    {
        if (rank == 0)
        {
            fprintf(stderr, "Number of processors must be a power of two.\n");
        }
        MPI_Finalize();
        exit(EXIT_FAILURE);
    }

    double local_sum = my_value;
    MPI_Status status;

    // Tree-based reduction
    for (int step = 1, phase = 0; step < size; step *= 2, phase++)
    {
        if (rank % (2 * step) == 0)
        {
            double received_value;
            MPI_Recv(&received_value, 1, MPI_DOUBLE, rank + step, 0, MPI_COMM_WORLD, &status);
            printf(" Phase %d - P %d (%02d) receiving from P %d (%02d), val %.1f\n", phase, rank, rank, rank + step, rank + step, received_value);
            local_sum += received_value;
        }
        else if (rank % step == 0)
        {
            MPI_Ssend(&local_sum, 1, MPI_DOUBLE, rank - step, 0, MPI_COMM_WORLD);
            printf(" Phase %d - P %d (%02d) sending   to   P %d (%02d), val %.1f\n", phase, rank, rank, rank - step, rank - step, local_sum);
            break;
        }
    }

    // Broadcast the result
    for (int step = size / 2, phase = 0; step >= 1; step /= 2, phase++)
    {
        if (rank % (2 * step) == 0)
        {
            MPI_Ssend(&local_sum, 1, MPI_DOUBLE, rank + step, 0, MPI_COMM_WORLD);
            printf(" Phase %d - P %d (%02d) sending   to   P %d (%02d), val %.1f\n", phase, rank, rank, rank + step, rank + step, local_sum);
        }
        else if (rank % step == 0)
        {
            MPI_Recv(&local_sum, 1, MPI_DOUBLE, rank - step, 0, MPI_COMM_WORLD, &status);
            printf(" Phase %d - P %d (%02d) receiving from P %d (%02d), val %.1f\n", phase, rank, rank, rank - step, rank - step, local_sum);
        }
    }

    *result = local_sum;
}
