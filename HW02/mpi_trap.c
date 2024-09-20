#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>
#include "trap.h"

int main(int argc, char *argv[]) {
   int my_rank, comm_sz, func_choice; 
   double start, end, elapsed;
   long long int n, local_n;
   double a, b, h, local_a, local_b;
   double local_int, total_int;

   MPI_Init(NULL, NULL);
   MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
   MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

   start = MPI_Wtime();

   // Process 0 gathers the input
   if (my_rank == 0) {
       parse_input(argc, argv, my_rank, &a, &b, &n, &func_choice);
   }

   // Broadcast the input values to all processes
   MPI_Bcast(&a, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
   MPI_Bcast(&b, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
   MPI_Bcast(&n, 1, MPI_LONG_LONG_INT, 0, MPI_COMM_WORLD);
   MPI_Bcast(&func_choice, 1, MPI_INT, 0, MPI_COMM_WORLD);

   h = (b - a) / (double) n;  // Width of trapezoid
   local_n = BLOCK_SIZE(my_rank, comm_sz, n); // Number of trapezoids for this process
   local_a = a + BLOCK_LOW(my_rank, comm_sz, n) * h;
   local_b = local_a + local_n * h;

   // Pass func_choice to Trap
   local_int = Trap(local_a, local_b, local_n, h, func_choice);

   MPI_Reduce(&local_int, &total_int, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

   end = MPI_Wtime();
   elapsed = end - start;

   if (my_rank == 0) {
    // Print in a structured format: N, P, a, b, integral result, time
    printf("%lld,%d,%f,%f,%.15f,%.6f\n", n, comm_sz, a, b, total_int, elapsed);
}

   MPI_Finalize();
   return 0;
}
