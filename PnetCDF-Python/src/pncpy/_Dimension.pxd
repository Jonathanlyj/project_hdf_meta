from ._File cimport File

cdef class Dimension:
    cdef public int _id, _fileid
    cdef public File _file
    cdef public _name, _data_model

    """
    A netCDF `Dimension` is used to describe the coordinates of a `Variable`.
    See `Dimension.__init__` for more details.
    The current maximum size of a `Dimension` instance can be obtained by
    calling the python `len` function on the `Dimension` instance. The
    `Dimension.isunlimited` method of a `Dimension` instance can be used to
    determine if the dimension is unlimited.
    Read-only class variables:
    **`name`**: String name, used when creating a `Variable` with
    `Dataset.createVariable`.
    **`size`**: Current `Dimension` size (same as `len(d)`, where `d` is a
    `Dimension` instance).
    """
