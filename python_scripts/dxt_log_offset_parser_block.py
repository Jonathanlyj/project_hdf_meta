import sys
import os
from matplotlib import pyplot as plt

def collapse(reads):
    size = len(reads)
    new_reads = []
    i = 0
    while i < size:
        tot = reads[i][1]
        start = reads[i][0]
        while (i < size - 1) and (start + tot == reads[i + 1][0]):
            tot += reads[i + 1][1]
            i += 1
        new_reads.append((start, tot))
        i += 1
    return new_reads

def merge_block(reads):
    size = len(reads)
    new_reads = []
    i = 0
    while i < size:
        tot = reads[i][1]
        start = reads[i][0]
        while (i < size - 1) and (start + tot >= reads[i + 1][0]):
            tot = reads[i + 1][1] + reads[i + 1][0] - start
            i += 1
        new_reads.append((start, tot))
        i += 1
    return new_reads






# test_reads = [(0, 8), (0, 16), (16, 80), (96, 512), (600, 512)]

# print(merge_block(test_reads))

filename = sys.argv[1]
 

with open(filename) as f:
    lines = f.readlines()

reads = []
for l in lines:
    # print(l)
    if l.startswith(" X_POSIX"):
        strings = l.strip().split()
        # print(strings)
        reads.append((int(strings[4]), int(strings[5])))
reads = sorted(reads, key = lambda x: x[0])
nonconsec_reads = collapse(reads)
merge_reads = merge_block(reads)
print(f"number of nonconsec_read: {len(nonconsec_reads)}")
print(f"number of consec_read: {len(reads) - len(nonconsec_reads)}")

print(f"number of merged_read: {len(merge_reads)}")
# print(merge_reads)

block_offsets = [r[0] for r in nonconsec_reads]
# print(f"non-consecutive ranges: {min(block_offsets)} ~ {max(block_offsets)}")
# max_range = os.path.getsize(filename)

# plt.hist(block_offsets, bins = 100, range = (0, 12246336))
# plt.hist(block_offsets, bins = 100)
# plt.xlabel("Non-consecutive reads offsets")
# plt.ylabel("Count")

# img_name = filename.replace(".txt", ".png")
# plt.savefig(img_name)





# nonconsec_reads = sorted(nonconsec_reads, key = lambda x: x[0])
# print(len(nonconsec_reads))


# Save new offsets and lengths and reads to new txt file
new_filename = filename.replace(".txt", "_raw_sorted.txt")
with open(new_filename, "w") as f:
    f.write("{:<15}".format("Segment"))
    f.write("{:<15}".format("Offset"))
    f.write("{:<15}\n".format("Length"))
    for i in range(len(reads)):
        f.write("{:<15}".format(i))
        f.write("{:<15}".format(reads[i][0]))
        f.write("{:<15}\n".format(reads[i][1]))

new_filename = filename.replace(".txt", "_coalesce_sorted.txt")
with open(new_filename, "w") as f:
    f.write("{:<15}".format("Segment"))
    f.write("{:<15}".format("Offset"))
    f.write("{:<15}\n".format("Length"))
    for i in range(len(nonconsec_reads)):
        f.write("{:<15}".format(i))
        f.write("{:<15}".format(nonconsec_reads[i][0]))
        f.write("{:<15}\n".format(nonconsec_reads[i][1]))


new_filename = filename.replace(".txt", "_merged.txt")
with open(new_filename, "w") as f:
    f.write("{:<15}".format("Segment"))
    f.write("{:<15}".format("Offset"))
    f.write("{:<15}\n".format("Length"))
    for i in range(len(merge_reads)):
        f.write("{:<15}".format(i))
        f.write("{:<15}".format(merge_reads[i][0]))
        f.write("{:<15}\n".format(merge_reads[i][1]))







        
