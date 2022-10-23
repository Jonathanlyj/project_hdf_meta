#include "hdf5.h"
#include <stdio.h>
#include <time.h>

#define FILE       "train64.h5"
#define META_BLOCK_SIZE  4096


/*
 * Operator function to be called by H5Lvisit.
 */
herr_t print_link (hid_t loc_id, const char *name, const H5L_info_t *info,
            void *operator_data);




int
main (void)
{
    hid_t           file, fapl;           
    herr_t          status;

    clock_t start, end;
    double time_used;
    /*
     * Set metadata block size
     */
    fapl = H5Pcreate(H5P_FILE_ACCESS);
    H5Pset_meta_block_size(fapl, META_BLOCK_SIZE);
    /*
     * Open file
     */
    file = H5Fopen (FILE, H5F_ACC_RDONLY, fapl);
    

    /*
     * Begin iteration using H5Lvisit
     */
    printf ("Objects in the file:\n");
    start = clock();    
    status = H5Lvisit (file, H5_INDEX_NAME, H5_ITER_NATIVE, print_link, NULL);

    end = clock();
    time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Metadata block size: %d\n", META_BLOCK_SIZE);
    printf("Traverse took %f seconds to execute \n", time_used);

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
herr_t print_link (hid_t loc_id, const char *name, const H5L_info_t *info,
            void *operator_data)
{
    herr_t          status;
    H5O_info_t      infobuf;
    printf ("/");               /* Print root group in object path */



    status = H5Oget_info_by_name(loc_id, name, &infobuf, H5P_DEFAULT);

    if (name[0] == '.')         /* Root group, do not print '.' */
        printf ("  (Group)\n");
    else
        switch (infobuf.type) {
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


