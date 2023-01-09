# Pnetcdf-python
## Setup
### Requirements
* Python 3.9+

## Development installation
* Clone GitHub repository 

* Make sure [numpy](http://www.numpy.org/) and [Cython](http://cython.org/) are
  installed and you have [Python](https://www.python.org) 3.9 or newer.

* Make sure [PnetCDF](https://github.com/Parallel-NetCDF/PnetCDF) are installed, 
  and the filepath of `pnetcdf-config` filepath is added to `setup.cfg`

* Run `env CC=mpicc python3 setup.py build`, then `env CC=mpicc python3 setup.py install`

## Development tasks
- [ ] Generic tasks
    - [x] The implementation Cython file (.pyx) splitted by classes(_File.pyx, Dimension.pyx)
    - [ ] Add utils Cython files to include all helper functions and import them in class implementation files
    - [ ] Fix "not found" error with direct importing pncpy module

- [ ] File API implementation
    - [x] Implement and test basic functions (create, open, close) 
    - [x] Implement and test Cmode and data format option 
    - [x] Implement inquiry functions related to file and dimension
    - [x] Implement and test methods to enter/exit define mode 
    - [x] Implement and test dimension operation methods
    - [ ] Attributes, variables ...

- [ ] Dimension API implementation
    - [x] Implement and test dimension class constructor
    - [ ] Comprehensively test all dimension methods

- [ ] Attribute API implementation

- [ ] Variable API implementation
    
## Design features
- Cython implmentation file (.pyx) partitioned by class. Each class has a seperate declaration file and implementation file
- Enabled both singular and bulk options for Define Mode
    - Singular option: enter/exit Define Mode to modify a single attribute/dimension/variable. Usually triggered indirectly by a create/modify method of a File instance
    - Bulk option: user proactively enter/exit Define Mode of a File instance before/after multiple modifications to attributes/dimensions/variables so that Define Mode is entered only once thoughout the process


## References and learning materials
- [Python C extension build tutorial][https://realpython.com/build-python-c-extension-module]
- [Cython basics][https://cython.readthedocs.io/en/latest/src/userguide/language_basics.html]
- [netCDF4 - Python repo][https://github.com/Unidata/netcdf4-python]
- [h5py repo][https://github.com/h5py/h5py]