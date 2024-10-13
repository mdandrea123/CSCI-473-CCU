#!/bin/bash
#SBATCH --job-name=neutron_perf
#SBATCH --output=neutron_perf.%j.out
#SBATCH --partition=compute
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=64
#SBATCH --mem=4G
#SBATCH --account=ccu108
#SBATCH --export=ALL
#SBATCH -t 01:30:00
#SBATCH --mail-user=mdandrea@coastal.edu
#SBATCH --mail-type=BEGIN,END

module purge
module load cpu
module load slurm
module load gcc/10.2.0/npcyll4
module load openmpi/4.1.1
module load py-matplotlib/3.4.3/yi6zmvu
module load py-pandas/1.3.4/xzzqb4t

make clean all

python gather_performance_data.py performance_data.csv