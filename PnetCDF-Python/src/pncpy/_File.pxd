from ._Dimension cimport Dimension

cdef class File:
    cdef int ierr
    cdef public int _ncid
    cdef public int _isopen, _def_mode_on
    cdef public data_model, path, dimensions
