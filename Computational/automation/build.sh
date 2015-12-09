cd setup/
module load ansys
mpirun -np 2 fluent 2ddp -g -np2 < udf_compile.tui
