#!/bin/bash
# srun --partition=debug  --pty --account=ccu108 --nodes=1 --ntasks-per-node=2 --mem=1G -t 00:30:00 --wait=0 --export=ALL /bin/bash

module purge
module load cpu/0.17.3b
module load slurm
module load gcc/10.2.0/npcyll4
module load openmpi/4.1.1/ygduf2r
