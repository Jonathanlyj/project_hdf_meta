import sys
import os
__version__ = "0.0.1"

include "PnetCDF.pxi"

cimport mpi4py.MPI as MPI
from mpi4py.libmpi cimport MPI_Comm, MPI_Info, MPI_Comm_dup, MPI_Info_dup, \
                               MPI_Comm_free, MPI_Info_free, MPI_INFO_NULL,\
                               MPI_COMM_WORLD
ctypedef MPI.Comm Comm
ctypedef MPI.Info Info

cdef class File:
    cdef int ierr
    cdef public int _ncid
    cdef public int _isopen

    def __init__(self, filename, format='64BIT_OFFSET', clobber=True, mode="w", Comm comm=None, Info info=None, **kwargs):
        """
        **`_init__(self, filename, Comm comm=None, Info info=None, **kwargs)`**

        `File` constructor.

        **`filename`**: Name of netCDF file to hold dataset.

        **`mode`**: access mode. `r` means read-only; no data can be
        modified. `w` means write; a new file is created, an existing file with
        the same name is deleted. `x` means write, but fail if an existing
        file with the same name already exists. `a` and `r+` mean append; 
        an existing file is opened for reading and writing, if 
        file does not exist already, one is created.


        **`clobber`**: if `True` (default), opening a file with `mode='w'`
        will clobber an existing file with the same name.  if `False`, an
        exception will be raised if a file with the same name already exists.
        mode=`x` is identical to mode=`w` with clobber=False.

        **`format`**: underlying file format (one of `'64BIT_OFFSET'` or
        `'64BIT_DATA'`.
        Only relevant if `mode = 'w'` (if `mode = 'r','a'` or `'r+'` the file format
        is automatically detected). 
        """
        cdef int ncid
        encoding = sys.getfilesystemencoding()
        cdef char* path
        cdef MPI_Comm mpicomm = MPI_COMM_WORLD
        cdef MPI_Info mpiinfo = MPI_INFO_NULL
        if comm is not None:
            mpicomm = comm.ob_mpi
        if info is not None:
            mpiinfo = info.ob_mpi
        bytestr = _strencode(filename, encoding=encoding)
        path = bytestr

        supported_formats = ["64BIT_OFFSET", "64BIT_DATA"]

        if format not in supported_formats:
            msg="underlying file format must be one of `'64BIT_OFFSET'` or `'64BIT_DATA'`"
            raise ValueError(msg)

        # mode='x' is the same as mode='w' with clobber=False
        if mode == 'x':
            mode = 'w'; clobber = False   

        if mode == 'w' or (mode in ['a','r+'] and not os.path.exists(filename)):   
        
            if clobber:
                if format  == '64BIT_OFFSET':
                    with nogil:
                        ierr = ncmpi_create(mpicomm, path, NC_64BIT_OFFSET, mpiinfo, &ncid)
                else:
                    with nogil:
                        ierr = ncmpi_create(mpicomm, path, NC_64BIT_DATA, mpiinfo, &ncid)

            else:
                if format  == '64BIT_OFFSET':
                    with nogil: 
                        ierr = ncmpi_create(mpicomm, path, NC_NOCLOBBER | NC_64BIT_OFFSET, mpiinfo, &ncid)
                else:
                    with nogil:
                        ierr = ncmpi_create(mpicomm, path, NC_NOCLOBBER | NC_64BIT_DATA, mpiinfo, &ncid)
        
        elif mode == "r":
            with nogil:
                ierr = ncmpi_open(mpicomm, path, NC_NOWRITE, mpiinfo, &ncid)

        elif mode in ['a','r+'] and os.path.exists(filename):
            with nogil:
                ierr = ncmpi_open(mpicomm, path, NC_WRITE, mpiinfo, &ncid)
        else:
            raise ValueError("mode must be 'w', 'x', 'r', 'a' or 'r+', got '%s'" % mode)


        _check_err(ierr, err_cls=OSError, filename=path)
        self._isopen = 1
        self._ncid=ncid
    
    def close(self):
        self._close(True)
    
    def _close(self, check_err):
        cdef int ierr
        with nogil:
            ierr = ncmpi_close(self._ncid)

        if check_err:
            _check_err(ierr)

        self._isopen = 0 # indicates file already closed, checked by __dealloc__

    def __dealloc__(self):
        # close file when there are no references to object left
        if self._isopen:
           self._close(False)

    def __enter__(self):
        return self
    def __exit__(self,atype,value,traceback):
        self.close()

cdef _strencode(pystr,encoding=None):
    # encode a string into bytes.  If already bytes, do nothing.
    # uses 'utf-8' for default encoding.
    if encoding is None:
        encoding = 'utf-8'
    try:
        return pystr.encode(encoding)
    except (AttributeError, UnicodeDecodeError):
        return pystr # already bytes or unicode?

cdef _check_err(ierr, err_cls=RuntimeError, filename=None):
    # print netcdf error message, raise error.
    if ierr != NC_NOERR:
        err_str = (<char *>ncmpi_strerror(ierr)).decode('ascii')
        if issubclass(err_cls, OSError):
            if isinstance(filename, bytes):
                filename = filename.decode()
            raise err_cls(ierr, err_str, filename)
        else:
            raise err_cls(err_str)