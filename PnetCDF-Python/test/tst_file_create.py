from mpi4py import MPI
from pncpy._File import File

comm = MPI.COMM_WORLD
rank = comm.Get_rank()
size = comm.Get_size()

file = File(filename="tst_file_create.nc", Comm=comm, Info=None)

file.close()

