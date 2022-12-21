#!/bin/bash
# Slurm batch file

#SBATCH --nodes=1
#SBATCH --tasks-per-node=10
#ingnore--ntasks=24
#SBATCH --time=01:00:00
#SBATCH --job-name=MPI-Matrix-Mult

module purge
module load OpenMPI/4.1.4

make
