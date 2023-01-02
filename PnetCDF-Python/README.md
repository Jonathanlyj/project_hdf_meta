# PnetCDF-Python

## install
1. Configure pnetcdf_config in set.cfg 
2. Build & install pncpy module
    ```shell
    pip install numpy
    pip install Cython
    CC=mpicc pip install mpi4py
    CC=mpicc python setup.py build
    CC=mpicc python setup.py install
    ```