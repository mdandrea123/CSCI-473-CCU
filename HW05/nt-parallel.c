#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <mpi.h>
#include <time.h>

void simulate_neutrons(double A, double C, double H, int n, int *local_results);

int main(int argc, char *argv[]) {
    int rank, size;
    double A = 0.0, C = 0.0, H = 0.0;
    int n = 0;
    int opt;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Parse command-line arguments (only process 0 needs to do this)
    if (rank == 0) {
        while ((opt = getopt(argc, argv, "A:C:H:n:")) != -1) {
            switch (opt) {
                case 'A': A = atof(optarg); break;
                case 'C': C = atof(optarg); break;
                case 'H': H = atof(optarg); break;
                case 'n': n = atoi(optarg); break;
                default:
                    fprintf(stderr, "Usage: %s -A <value> -C <value> -H <value> -n <value>\n", argv[0]);
                    MPI_Abort(MPI_COMM_WORLD, 1);
            }
        }

        if (A == 0.0 || C == 0.0 || H == 0.0 || n == 0) {
            fprintf(stderr, "All arguments (-A, -C, -H, -n) are required.\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
    }

    // Broadcast parameters to all processes
    MPI_Bcast(&A, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(&C, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(&H, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Calculate local number of neutrons to simulate
    int local_n = n / size;
    if (rank < n % size) local_n++;

    // Simulate neutrons
    int local_results[3] = {0}; // [reflected, absorbed, transmitted]
    simulate_neutrons(A, C, H, local_n, local_results);

    // Reduce results
    int global_results[3] = {0};
    MPI_Reduce(local_results, global_results, 3, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    // Print results (only process 0)
    if (rank == 0) {
        double total = (double)(global_results[0] + global_results[1] + global_results[2]);
        printf("Reflected: %.2f%%\n", (global_results[0] / total) * 100.0);
        printf("Absorbed:  %.2f%%\n", (global_results[1] / total) * 100.0);
        printf("Transmitted: %.2f%%\n", (global_results[2] / total) * 100.0);
    }

    MPI_Finalize();
    return 0;
}

void simulate_neutrons(double A, double C, double H, int n, int *local_results) {
    unsigned short seed[3];
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Initialize the random number generator seed based on rank
    seed[0] = (unsigned short)(time(NULL) ^ rank);
    seed[1] = (unsigned short)((time(NULL) ^ rank) >> 16);
    seed[2] = (unsigned short)((time(NULL) ^ rank) >> 32);

    for (int i = 0; i < n; i++) {
        double x = 0.0; // Initial position
        double mu = 2.0 * erand48(seed) - 1.0; // Initial direction cosine

        while (1) {
            double d = -log(erand48(seed)) / C; // Distance to collision
            x += d * mu; // Update position

            if (x < 0.0) {
                local_results[0]++; // Reflected
                break;
            } else if (x > H) {
                local_results[2]++; // Transmitted
                break;
            }

            if (erand48(seed) < A) {
                local_results[1]++; // Absorbed
                break;
            }

            mu = 2.0 * erand48(seed) - 1.0; // New direction cosine
        }
    }
}
