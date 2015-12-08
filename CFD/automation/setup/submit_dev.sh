#!/bin/bash
#PBS -V
#PBS -l walltime=2:00:00
#PBS -l nodes=1:ppn=4
#PBS -W group_list=ithaca
#PBS -q normal_q
#PBS -M pid@vt.edu
#PBS -m bea
cd $PBS_O_WORKDIR
module load ansys
fluent 2ddp -t$PBS_NP -pinfiniband -cnf=$PBS_NODEFILE -g -ssh < setup/simulate_fin.tui
