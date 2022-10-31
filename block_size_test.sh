

# compile h5 file duplication program file_dup_b_size.c
h5cc C_scripts/file_dup_b_size.c -o C_scripts/file_dup_b_size

#set metadata block size and recreate target h5 file
IFS='.'
read -a strarr <<< "$1"
new_file=${strarr[0]}"_"$2"."${strarr[1]}
echo "./C_scripts/file_dup_b_size $1 $new_file $2"
./C_scripts/file_dup_b_size "$1" "$new_file" $2

# compile h5 file reading program file_iter_l_name.c
h5cc C_scripts/file_iter_l_name.c -o C_scripts/file_iter_l_name
#read new h5 file, print all objects names and track I/O using darshan
export DARSHAN_ENABLE_NONMPI=1 #for non-MPI io
env LD_PRELOAD=/homes/yll6162/darshan/darshan-install/lib/libdarshan.so ./C_scripts/file_iter_l_name "$new_file" $2

#find the darshan log created
log_file=`find $DARSHAN_LOG_DIR -name "*file_iter_l_name*.darshan" -cmin -1 | sort -r | head -n 1`
echo "Found log file: $log_file"
#analyze darshan logs and generate report
cd darshan_reports
python3 -m darshan summary "$log_file"





# echo "The file name: $0."
# echo "The first argument is $1."
# echo "The second argument is $2."
# echo "The third argument is $3."

