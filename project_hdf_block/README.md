## HDF5 File Exploration
#### Built with
- hdf5-1.10.9
### Intro
The repo collects several programs in C or python that explores HDF5 API. The overall goal is to develop programs to tune metadata_block_size to optimize h5 file IO runtime.
### Pipeline
A shell script is developed to automate the following process
1. Recreate target h5 file with different metadata block sizes as a parameter
2. Read the new h5 file, iterate over all objects and print all object names
3. Track I/O process for task 2 using darshan
4. Locate the corresponding log file and parse the log  

`block_size_test.sh` [target h5 file]  [metadata block size]  
The current shell script requires the target h5 file existed under the top directory.

### Main Programs (Outdated)
Following C programs explore the sample h5 file achieved by HDF5 API. The current program requires target h5 file (name specified by FILE macro) existed under the same file directory.
- `dset_dup.c`  
Simply read the dataset of a 2d array (datatype known as float) from h5 file and rewrite to a new h5 file.
- `file_dup.c`  
Traverse the source h5 file and recreate groups and datasets under a new h5 file.
- `file_iter_o_name.c`  
Iterate over all objects (groups and datasets) under the h5 file and print their names with total execution time recorded. The metadata block size and specified and used in H5FOpen. 
- `file_iter_l_name.c`
Similar to above but iterate over links instead of objects. An extra step (H5Oget_info_by_name) is taken during iteration to explicitly retrieve object metadata. 






