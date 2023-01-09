from mpi4py import MPI
from pncpy._File import File

comm = MPI.COMM_WORLD
rank = comm.Get_rank()
size = comm.Get_size()

file = File(filename="tst_file_open.nc", mode='w', Comm=comm, Info=None)
file.close()

file = File(filename="tst_file_open.nc", mode='r', Comm=comm, Info=None)
file.close()
