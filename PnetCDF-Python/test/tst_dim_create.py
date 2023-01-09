from mpi4py import MPI
from pncpy._File import File
from pncpy._Dimension import Dimension

comm = MPI.COMM_WORLD
rank = comm.Get_rank()
size = comm.Get_size()

file1 = File(filename="tst_dim_create.nc", mode='w', Comm=comm, Info=None)
file1.createDimension(dimname = "dummy_dim1", size = 3)
file1.createDimension(dimname = "dummy_dim2", size = None)
print(file1.dimensions)
file1.close()