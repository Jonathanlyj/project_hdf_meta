#include "hdf5.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define FILE            "train64.h5"
#define DATASET         "x_v"
#define GROUP           "/9"
#define FILE1           "train64_cp.h5"
// #define DIM0            4
// #define ADIM0           3
// #define ADIM1           5

int main(void){

    hid_t       file, group, dspace, dset, dtype, dtype_class;  
    hid_t       w_file, w_group, w_dspace, w_dset;
    herr_t      status;
    hsize_t*     dims;
    hsize_t arraysize;
    int a;
    int rank;
    int i,j;

    //Read
    file = H5Fopen (FILE, H5F_ACC_RDONLY, H5P_DEFAULT);
    group = H5Gopen (file, GROUP, H5P_DEFAULT);
    dset = H5Dopen (group, DATASET, H5P_DEFAULT);
    dspace = H5Dget_space(dset);

    rank = H5Sget_simple_extent_ndims(dspace);
    dims = (hsize_t *)malloc(rank * sizeof(hsize_t));
    dtype = H5Dget_type(dset);
    dtype_class  = H5Tget_class(dtype);
    H5Sget_simple_extent_dims(dspace, dims, NULL);

    printf("rank %d, dimensions %d x %d \n", rank, dims[0], dims[1]);

    arraysize = 1;
    for(i=0; i < rank; i++){
        arraysize *= dims[i];
    }

    // rdata = (float **)malloc(arraysize * sizeof(float));
    float rdata[dims[0]][dims[1]];
    status = H5Dread (dset, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, H5P_DEFAULT,
                rdata[0]);

    status = H5Tclose(dtype);
    status = H5Dclose(dset);
    status = H5Gclose(group);
    status = H5Fclose(file);

    //Write
    float wdata[dims[0]][dims[1]];
    for (i=0; i<dims[0]; i++) {
    for (j=0; j<dims[1]; j++)
        wdata[i][j] = rdata[i][j];
    }     

    w_file = H5Fcreate (FILE1, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    w_group = H5Gcreate (w_file, GROUP, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    w_dspace = H5Screate_simple (2, dims, NULL);

    w_dset = H5Dcreate (w_group, DATASET, H5T_IEEE_F32LE, w_dspace, H5P_DEFAULT,
                H5P_DEFAULT, H5P_DEFAULT);
    status = H5Dwrite (w_dset, H5T_IEEE_F32LE, H5S_ALL, H5S_ALL, H5P_DEFAULT,
                wdata[0]);

    status = H5Dclose(w_dset);
    status = H5Sclose(w_dspace);
    status = H5Gclose(w_group);
    status = H5Fclose(w_file);

}
