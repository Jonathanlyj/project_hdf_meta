from mpi4py import MPI
from pncpy._File import File
from pncpy._Dimension import Dimension

comm = MPI.COMM_WORLD
rank = comm.Get_rank()
size = comm.Get_size()

file1 = File(filename="tst_dim_create.nc", mode='w', Comm=comm, Info=None)

dim_name = "dummy_dim"
dim_size = 3
dim = Dimension(file = file1, size = dim_size, name = dim_name)
file1.close()

