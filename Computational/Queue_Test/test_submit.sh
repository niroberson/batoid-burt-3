#!/bin/bash
#PBS -V
#PBS -l walltime=000:10:00
#PBS -l nodes=1:ppn=4
#PBS -W group_list=ithaca
#PBS -q normal_q
#PBS -M pid@vt.edu
#PBS -m bea
cd $PBS_O_WORKDIR
module load ansys
#fluent 2d -t$PBS_NP -pinfiniband -cnf=$PBS_NODEFILE -g -ssh < fluent_journal_name

# Define loop parameters
numsimulations=32
numtasks=16
numgen=2
idxGen=1
while [ $idxGen -le $numgen ]
	do
	idxTask=1
	while [ $idxTask -le $numtasks ]
		do
		mpirun -np 2 echo "Task:"  $idxTask "Gen:" $idxGen >> a.out &
		idxTask=$((idxTask+1))
		done
	idxGen=$((idxGen+1))
	wait
	done
