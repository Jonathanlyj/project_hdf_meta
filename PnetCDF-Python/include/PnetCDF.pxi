# # size_t, ptrdiff_t are defined in stdlib.h
# cdef extern from "stdlib.h":
#    ctypedef long size_t
#    ctypedef long ptrdiff_t

cdef extern from *:
    ctypedef char* const_char_ptr "const char*"
 
# netcdf functions.
# TODO: Check nogil
cdef extern from "pnetcdf.h":
    ctypedef int MPI_Comm
    ctypedef int MPI_Info
    const_char_ptr ncmpi_strerror(int err);
    cdef enum:
        NC_NOCLOBBER # Don't destroy existing file on create
        NC_64BIT_OFFSET
        NC_64BIT_DATA
        NC_NOWRITE
        NC_WRITE
        NC_NOERR
    int ncmpi_create(MPI_Comm comm, const char *path, int cmode, MPI_Info info, int *ncidp) nogil
    int ncmpi_open(MPI_Comm comm, const char *path, int cmode, MPI_Info info, int *ncidp) nogil
    int ncmpi_close(int ncid) nogil
