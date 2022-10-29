#include "hdf5.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>



/*
 * Operator function to be called by H5Ovisit.
 */
herr_t print_object (hid_t loc_id, const char *name, const H5O_info_t *info,
            void *operator_data);


int
main (int argc, char* argv[])
{
    hid_t           file, file_fapl;           
    herr_t          status;
    hsize_t block_size;

    clock_t start, end;
    double time_used;
    

    if (argc < 2){
        fprintf(stderr, "requires file directory \n");
        exit(-1);
    }

    char* file_name = argv[1];
    /*
     * Open file
     */
    file = H5Fopen (file_name, H5F_ACC_RDONLY, H5P_DEFAULT);


    

    /*
     * Begin iteration using H5Ovisit
     */
    printf ("Objects in the file:\n");
    start = clock();    
    status = H5Ovisit (file, H5_INDEX_NAME, H5_ITER_NATIVE, print_object, NULL);
    end = clock();
    time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Traverse took %f seconds to execute \n", time_used);
    file_fapl = H5Fget_access_plist(file);
    H5Pget_meta_block_size(file_fapl, &block_size);
    printf("Metadata block size: %d\n", block_size);

    /*
     * Close and release resources.
     */
    status = H5Fclose (file);

    return 0;
}


/************************************************************

  Operator function for H5Ovisit.  This function prints the
  name and type of the object passed to it.

 ************************************************************/
herr_t print_object (hid_t loc_id, const char *name, const H5O_info_t *info,
            void *operator_data)
{
    printf ("/");               /* Print root group in object path */

    /*
     * Check if the current object is the root group, and if not print
     * the full path name and type.
     */
    if (name[0] == '.')         /* Root group, do not print '.' */
        printf ("  (Group)\n");
    else
        switch (info->type) {
            case H5O_TYPE_GROUP:
                printf ("%s  (Group)\n", name);
                break;
            case H5O_TYPE_DATASET:
                printf ("%s  (Dataset)\n", name);
                break;
            case H5O_TYPE_NAMED_DATATYPE:
                printf ("%s  (Datatype)\n", name);
                break;
            default:
                printf ("%s  (Unknown)\n", name);
        }

    return 0;
}


