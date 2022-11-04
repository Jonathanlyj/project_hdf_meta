#include "hdf5.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>


#define FILE       "train64.h5"
#define META_BLOCK_SIZE  4096


/*
 * Operator function to be called by H5Lvisit.
 */
herr_t print_link (hid_t loc_id, const char *name, const H5L_info_t *info,
            void *operator_data);




int
main (int argc, char* argv[])
{
    hid_t           file, file_fapl, new_file_fapl;           
    herr_t          status;
    hsize_t block_size, new_block_size;

    clock_t start, end;
    double time_used;

    unsigned long flags;
    const char* driver_info;

    if (argc < 3){
        fprintf(stderr, "requires file name and block size \n");
        exit(-1);
    }

    char* file_name = argv[1];
    block_size = (int) atoi(argv[2]);
    /*
     * Open file
     */

    file_fapl = H5Pcreate(H5P_FILE_ACCESS);
    H5Pset_meta_block_size(file_fapl, block_size);
    file = H5Fopen (file_name, H5F_ACC_RDONLY, file_fapl);


    /*
     * Begin iteration using H5Ovisit
     */
    printf ("Links in the file:\n");
    start = clock();    
    status = H5Lvisit (file, H5_INDEX_NAME, H5_ITER_NATIVE, print_link, NULL);
    end = clock();
    time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Traverse %s took %f seconds to execute \n", file_name, time_used);
    new_file_fapl = H5Fget_access_plist(file);
    H5Pget_meta_block_size(new_file_fapl, &new_block_size);
    printf("Metadata block size: %d\n", new_block_size);

    /*
     * Get file driver info
     */
    hid_t driver = H5Pget_driver(new_file_fapl);
    H5FDdriver_query (driver, &flags);
    printf("query result: %x \n", flags);

    

    if (H5FD_SEC2==driver) {
        printf("File driver: H5FD_SEC2\n");
    } else if(H5FD_MULTI==driver){
        printf("File driver: H5FD_MULTI\n");
    }

    /*
     * Close and release resources.
     */
    status = H5Fclose (file);

    return 0;
}


/************************************************************

  Operator function for H5Lvisit.  This function prints the
  name and type of the object passed to it.

 ************************************************************/
herr_t print_link (hid_t loc_id, const char *name, const H5L_info_t *info,
            void *operator_data)
{
    herr_t          status;
    H5O_info_t      infobuf;
    // printf ("/");               /* Print root group in object path */



    status = H5Oget_info_by_name(loc_id, name, &infobuf, H5P_DEFAULT);

    if (name[0] == '.')         /* Root group, do not print '.' */
        printf ("  (Group)\n");
    else
        switch (infobuf.type) {
            case H5O_TYPE_GROUP:
                // printf ("%s  (Group)\n", name);
                break;
            case H5O_TYPE_DATASET:
                // printf ("%s  (Dataset)\n", name);
                break;
            case H5O_TYPE_NAMED_DATATYPE:
                printf ("%s  (Datatype)\n", name);
                break;
            default:
                printf ("%s  (Unknown)\n", name);
        }

    return 0;
}


