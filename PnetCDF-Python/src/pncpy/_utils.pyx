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
