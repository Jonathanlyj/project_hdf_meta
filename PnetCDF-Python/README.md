# PnetCDF-Python

## install
1. Configure pnetcdf_config in set.cfg
2. Build & install pncpy module
    ```shell
    pip install numpy
    pip install Cython
    env CC=mpicc pip3 install mpi4py
    env CC=mpicc python3 setup.py build
    env CC=mpicc python3 setup.py install
    ```