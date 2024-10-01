#!/bin/bash

# Clean and compile the project
make clean all

# Parameters
n_start=100000000        # Starting value for N
n_end=500000000     # Upper bound for N (based on your testing)
n_increment=100000000 # Increment N so that there are at least 5 values

# Output file
output_file="results.txt"
> $output_file

# Loop over values of N
for ((n=$n_start; n<=$n_end; n+=$n_increment))
do
    # Loop over values of P (number of processes)
    for p in {1..16}
    do
        echo "Running with N=$n and P=$p"
        mpirun --oversubscribe -np $p ./mpi_trap -n $n -a 0 -b 1 -f 3 >> $output_file
    done
done

echo "All simulations complete. Results saved in $output_file."
