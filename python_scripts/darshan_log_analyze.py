import darshan

# Open darshan log
filepath = "/homes/yll6162/darshan/darshan-logs/2022/10/30/yll6162_file_iter_o_name_id2560921-2560921_10-30-6256-856091647549899820_1.darshan"
report = darshan.DarshanReport(filepath, read_all=False)

# Load some report data
# report.mod_read_all_records('POSIX')
# report.mod_read_all_records('MPI-IO') 
# or fetch all
report.read_all_generic_records()

# ...
# Generate summaries for currently loaded data
# Note: aggregations are still experimental and have to be activated:
darshan.enable_experimental()
report.summarize()