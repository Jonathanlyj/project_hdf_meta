from ._File cimport File
cimport mpi4py.MPI as MPI

ctypedef MPI.Offset Offset
#TODO: Fix utils function import crash error
# from .utils cimport _strencode, _check_err

from mpi4py.libmpi cimport MPI_Offset

include "PnetCDF.pxi"

cdef class Dimension:
    def __init__(self, File file, name, Offset size, **kwargs):
        self._id = 0
        self._file = file
        """
        **`__init__(self, File file, name, Offset size = None, **kwargs)`**
        `Dimension` constructor.
        **`file`**: `File` instance to associate with dimension.
        **`name`**: Name of the dimension.
        **`size`**: Size of the dimension. `None` or 0 means unlimited. (Default `None`).
        ***Note***: `Dimension` instances should be created using the
        `Dataset.createDimension` method of a `Group` or
        `Dataset` instance, not using `Dimension.__init__` directly.
        """
        cdef int ierr
        cdef char *dimname
        cdef MPI_Offset lendim
        self._fileid = file._ncid
        self._data_model = file.data_model
        self._name = name

        if 'id' in kwargs:
        # TODO: Verify or delete this kind of initialization
            self._id = kwargs['id']
        else:
            bytestr = _strencode(name)
            dimname = bytestr
            if size is not None:
                lendim = size
            else:
                lendim = NC_UNLIMITED
            file.redef()
            with nogil:
                ierr = ncmpi_def_dim(self._fileid, dimname, lendim, &self._id)
            file.enddef()
            _check_err(ierr)

    def _getname(self):
        # private method to get name associated with instance.
        cdef int err, _fileid
        cdef char namstring[NC_MAX_NAME+1]

        with nogil:
            ierr = ncmpi_inq_dimname(self._fileid, self._id, namstring)
        _check_err(ierr)
        return namstring.decode('utf-8')

    property name:
        """string name of Dimension instance"""
        def __get__(self):
            return self._getname()
        def __set__(self,value):
            raise AttributeError("name cannot be altered")

    property size:
        """current size of Dimension (calls `len` on Dimension instance)"""
        def __get__(self):
            return len(self)
        def __set__(self,value):
            raise AttributeError("size cannot be altered")

    def __repr__(self):
        return self.__str__()

    def __str__(self):
        if not dir(self._file):
            return 'Dimension object no longer valid'
        if self.isunlimited():
            return "%r (unlimited): name = '%s', size = %s" %\
                (type(self), self._name, len(self))
        else:
            return "%r: name = '%s', size = %s" %\
                (type(self), self._name, len(self))

    def __len__(self):
        # len(`Dimension` instance) returns current size of dimension
        cdef int ierr
        cdef MPI_Offset lengthp
        with nogil:
            ierr = ncmpi_inq_dimlen(self._fileid, self._id, &lengthp)
        _check_err(ierr)
        return lengthp

    def getfile(self):
        """
**`file(self)`**

return the file that this `Dimension` is a member of."""
        return self._file

    def isunlimited(self):
        """
**`isunlimited(self)`**

returns `True` if the `Dimension` instance is unlimited, `False` otherwise."""
        cdef int ierr, n, numunlimdims, ndims, nvars, ngatts, xdimid
        cdef int *unlimdimids
        with nogil:
            ierr = ncmpi_inq(self._fileid, &ndims, &nvars, &ngatts, &xdimid)
        if self._id == xdimid:
            return True
        else:
            return False

#TODO: remove utils functions after import error fixed
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

