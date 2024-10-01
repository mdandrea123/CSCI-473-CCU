#!/bin/bash
#SBATCH --job-name=mpi_trap
#SBATCH --output=mpi_trap.%j.%N.out
#SBATCH --partition=compute
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=16
#SBATCH --mem=4G
#SBATCH --account=ccu108
#SBATCH --export=ALL
#SBATCH -t 00:10:00

module purge
module load cpu
module load slurm
module load gcc/10.2.0/npcyll4
module load openmpi/4.1.1

make clean all

n_start=10000        # Starting value for N
n_end=1500000000     # Upper bound for N (based on your testing)
n_increment=375000000 # Increment N so that there are at least 5 values

# Output file
output_file="results.txt"
> $output_file

# Loop over values of N
for ((n=$n_start; n<=$n_end + 10000; n+=$n_increment))
do
    # Loop over values of P (number of processes)
    for p in {1..16}
    do
        echo "Running with N=$n and P=$p"
        mpirun -np $p ./mpi_trap -n $n -a 0 -b 1 -f 1 >> $output_file
    done
done