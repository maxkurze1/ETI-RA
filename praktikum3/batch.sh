#!/bin/bash
# Slurm batch file

#SBATCH --nodes=1
#SBATCH --tasks-per-node=1
#SBATCH --cpus-per-task=4
#SBATCH --gres=gpu:1
#SBTACH --comment=no_monitoring
#SBATCH --time=2:00:00
#SBATCH --partition=ml

module purge
module load NVHPC
module load GCC/8.3.0

make profile
