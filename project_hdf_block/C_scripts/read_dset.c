#include "hdf5.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define FILE            "train64.h5"
#define DATASET         "x_v"
#define GROUP           "/9"
// #define DIM0            4
// #define ADIM0           3
// #define ADIM1           5

int main(void){

    hid_t       file, group, dspace, dset, dtype, dtype_class;  
    herr_t      status;
    hsize_t*     dims;
    hsize_t arraysize;
    int a;
    int rank;
    int i,j;


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

    for (i=0; i<dims[0]; i++) {
        printf (" [");
    for (j=0; j<dims[1]; j++)
            printf (" %6.3f", rdata[i][j]);
        printf ("]\n");
    }
    // a = H5Tequal(dtype, H5T_IEEE_F32LE);
    // printf("%d", a);

    status = H5Tclose(dtype);
    status = H5Dclose(dset);
    status = H5Gclose(group);
    status = H5Fclose(file);


}
