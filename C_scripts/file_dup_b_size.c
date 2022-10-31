#include "hdf5.h"
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>




/*
 * Operator function to be called by H5Ovisit.
 */
herr_t copy_object (hid_t loc_id, const char *name, const H5O_info_t *info,
            void *operator_data);

char group_name[1];
char* old_file;
char* new_file;
int
main (int argc, char* argv[])
{
    hid_t           file_id, fapl, d_file_id, file_fapl, new_file_fapl, d_file_fapl;           
    herr_t          status;

    clock_t start, end;
    double time_used;
    hsize_t block_size;
    hsize_t d_block_size;

    /*
     * Set metadata block size
     */



    if (argc < 3){
        printf(stderr, "requires old file directory and new file directory");
        exit(-1);
    }
    old_file = argv[1];
    new_file = argv[2];

    /*
     * Open old file and get metadata block size
     */
    file_id = H5Fopen (old_file, H5F_ACC_RDONLY, H5P_DEFAULT);
    file_fapl = H5Fget_access_plist(file_id);
    H5Pget_meta_block_size(file_fapl, &block_size);
    printf("Current file block size: %d\n", block_size);

    if (argc > 3) {
        block_size = (int) atoi(argv[3]);
    }

    /*
     * Create new file and set metadata block size
     */
    new_file_fapl = H5Pcreate(H5P_FILE_ACCESS);
    H5Pset_meta_block_size(new_file_fapl, block_size);
    d_file_id = H5Fcreate (new_file, H5F_ACC_TRUNC, H5P_DEFAULT, new_file_fapl);
    printf("New file created with block size: %d\n", block_size);
    
    status = H5Fclose(d_file_id);

    /*
     * Begin iteration using H5Ovisit
     */
    // printf ("Objects in the file:\n");
    // start = clock();    
    status = H5Ovisit (file_id, H5_INDEX_NAME, H5_ITER_NATIVE, copy_object, NULL);
    // end = clock();
    // time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    // printf("Metadata block size: %d\n", META_BLOCK_SIZE);
    // printf("Traverse took %f seconds to execute \n", time_used);
    d_file_id = H5Fopen(new_file, H5F_ACC_RDWR, new_file_fapl);
    d_file_fapl = H5Fget_access_plist(d_file_id);
    H5Pget_meta_block_size(d_file_fapl, &d_block_size);
    printf("Check new file metadata block size: %d\n", d_block_size);


    /*
     * Close and release resources.
     */
    status = H5Fclose (file_id);
    status = H5Fclose(d_file_id);


    return 0;
}


/************************************************************

  Operator function for H5Ovisit.  This function prints the
  name and type of the object passed to it.

 ************************************************************/
herr_t copy_object (hid_t loc_id, const char *name, const H5O_info_t *info,
            void *operator_data)
{
    // printf ("/");               /* Print root group in object path */

    /*
     * Check if the current object is the root group, and if not print
     * the full path name and type.
     */
    herr_t status;
    hid_t d_file_id, d_group;


    if (name[0] != '.'){          /* Root group, do not copy '.' */
        switch (info->type) {
            case H5O_TYPE_GROUP:
                // printf ("%s  (Group)\n", name);
                strcpy (group_name, name);
                d_file_id = H5Fopen(new_file, H5F_ACC_RDWR, H5P_DEFAULT);
                d_group = H5Gcreate (d_file_id, group_name, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
                // printf("group id is %d\n", d_group);
                status = H5Gclose (d_group);
                status = H5Fclose(d_file_id);
                break;

            case H5O_TYPE_DATASET:
                
                // printf ("%s  (Dataset)\n", name);
                // printf("group name is %s\n", group_name);
                d_file_id = H5Fopen(new_file, H5F_ACC_RDWR, H5P_DEFAULT);
                d_group = H5Gopen(d_file_id, group_name, H5P_DEFAULT);
                // printf("group id is %d\n", d_group);
                // printf("name is %s\n", name);
                H5Ocopy(loc_id, name, d_file_id, name, H5P_DEFAULT, H5P_DEFAULT);
                status = H5Gclose(d_group);
                status = H5Fclose(d_file_id);

                break;
            case H5O_TYPE_NAMED_DATATYPE:
                printf ("%s  (Datatype)\n", name);
                break;
            default:
                printf ("%s  (Unknown)\n", name);
        }
    }
    
    

    return 0;
}


