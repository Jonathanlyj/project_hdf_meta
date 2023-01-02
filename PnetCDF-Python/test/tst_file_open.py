from mpi4py import MPI
import pncpy

comm = MPI.COMM_WORLD
rank = comm.Get_rank()
size = comm.Get_size()

file = pncpy.File(filename="tst_file_create.nc", mode='r', Comm = comm, Info = None)
file.close()

