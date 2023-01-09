# # size_t, ptrdiff_t are defined in stdlib.h
# cdef extern from "stdlib.h":
#    ctypedef long size_t
#    ctypedef long ptrdiff_t

cdef extern from *:
    ctypedef char* const_char_ptr "const char*"

# Pnetcdf functions.
# TODO: Check nogil
cdef extern from "pnetcdf.h":
    ctypedef int MPI_Comm
    ctypedef int MPI_Info
    ctypedef int MPI_Offset
    const_char_ptr ncmpi_strerror(int err);
    cdef enum:
        NC_NOCLOBBER # Don't destroy existing file on create
        NC_64BIT_OFFSET
        NC_64BIT_DATA
        NC_NOWRITE
        NC_WRITE
        NC_NOERR
        # These maximums are enforced by the interface, to facilitate writing
        # applications and utilities.  However, nothing is statically allocated to
        # these sizes internally.
        NC_MAX_DIMS
        NC_MAX_ATTRS
        NC_MAX_VARS
        NC_MAX_NAME
        NC_MAX_VAR_DIMS
        # 'size' argument to ncdimdef for an unlimited dimension
        NC_UNLIMITED
    # File APIs
    int ncmpi_create(MPI_Comm comm, const char *path, int cmode, MPI_Info info, int *ncidp) nogil
    int ncmpi_open(MPI_Comm comm, const char *path, int cmode, MPI_Info info, int *ncidp) nogil
    int ncmpi_close(int ncid) nogil
    int ncmpi_enddef(int ncid) nogil
    int ncmpi_redef(int ncid) nogil
    # Dimension APIs
    int ncmpi_def_dim(int ncid, const char *name, MPI_Offset len, int *idp) nogil
    # Inquiry APIs
    int ncmpi_inq(int ncid, int *ndimsp, int *nvarsp, int *ngattsp, int *unlimdimidp) nogil
    int ncmpi_inq_ndims(int ncid, int *ndimsp) nogil
    int ncmpi_inq_dimlen(int ncid, int dimid, MPI_Offset *lenp) nogil
    int ncmpi_inq_dimname(int ncid, int dimid, char *name) nogil
