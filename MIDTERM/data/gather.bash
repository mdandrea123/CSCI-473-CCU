#!/bin/bash
#SBATCH --job-name="gather_data"
#SBATCH --output="gather_data.%j.%N.out"
#SBATCH --partition=compute
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --cpus-per-task=24
#SBATCH --mem=249325M
#SBATCH --account=ccu108
#SBATCH --export=ALL
#SBATCH -t 01:30:00

module load cpu/0.15.4
module load python3

python gather_data_threads.py
python gather_data_omp.py