#!/bin/bash
#PBS -V
#PBS -l walltime=2:00:00
#PBS -l nodes=1:ppn=4
#PBS -W group_list=blueridge
#PBS -q dev_q
#PBS -M pid@vt.edu
#PBS -m bea
cd $PBS_O_WORKDIR
module load ansys
fluent 2ddp -t$PBS_NP -pinfiniband -cnf=$PBS_NODEFILE -g -ssh < simulate_fin.tui
