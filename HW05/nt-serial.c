#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <time.h>

// Function prototypes
void simulate_neutrons(double A, double C, double H, int n);

int main(int argc, char *argv[]) {
    double A = 0.0, C = 0.0, H = 0.0;
    int n = 0;
    int opt;

    // Parse command-line arguments
    while ((opt = getopt(argc, argv, "A:C:H:n:")) != -1) {
        switch (opt) {
            case 'A':
                A = atof(optarg);
                break;
            case 'C':
                C = atof(optarg);
                break;
            case 'H':
                H = atof(optarg);
                break;
            case 'n':
                n = atoi(optarg);
                break;
            default:
                fprintf(stderr, "Usage: %s -A <value> -C <value> -H <value> -n <value>\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    // Check if all required arguments are provided
    if (A == 0.0 || C == 0.0 || H == 0.0 || n == 0) {
        fprintf(stderr, "All arguments (-A, -C, -H, -n) are required.\n");
        exit(EXIT_FAILURE);
    }

    // Run the simulation
    simulate_neutrons(A, C, H, n);

    return 0;
}

void simulate_neutrons(double A, double C, double H, int n) {
    int reflected = 0, absorbed = 0, transmitted = 0;
    unsigned short seed[3];

    // Initialize the random number generator seed
    seed[0] = (unsigned short)time(NULL);
    seed[1] = (unsigned short)((unsigned long)time(NULL) >> 16);
    seed[2] = (unsigned short)((unsigned long)time(NULL) >> 32);

    for (int i = 0; i < n; i++) {
        double x = 0.0; // Initial position
        double mu = 2.0 * erand48(seed) - 1.0; // Initial direction cosine

        while (1) {
            double d = -log(erand48(seed)) / C; // Distance to collision
            x += d * mu; // Update position

            if (x < 0.0) {
                reflected++;
                break;
            } else if (x > H) {
                transmitted++;
                break;
            }

            if (erand48(seed) < A) {
                absorbed++;
                break;
            }

            mu = 2.0 * erand48(seed) - 1.0; // New direction cosine
        }
    }

    // Calculate and print percentages
    double total = (double)(reflected + absorbed + transmitted);
    printf("Reflected: %.2f%%\n", (reflected / total) * 100.0);
    printf("Absorbed:  %.2f%%\n", (absorbed / total) * 100.0);
    printf("Transmitted: %.2f%%\n", (transmitted / total) * 100.0);
}