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

# test_reads = [(0, 8), (0, 16), (16, 80), (96, 512), (680, 512)]

# print(collapse(test_reads))

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
nonconsec_reads = collapse(reads)
print(f"number of nonconsec_read: {len(nonconsec_reads)}")
print(f"number of consec_read: {len(reads) - len(nonconsec_reads)}")

block_offsets = [r[0] for r in nonconsec_reads]
print(f"non-consecutive ranges: {min(block_offsets)} ~ {max(block_offsets)}")
# max_range = os.path.getsize(filename)

# plt.hist(block_offsets, bins = 100, range = (0, 12246336))
plt.hist(block_offsets, bins = 100)
plt.xlabel("Non-consecutive reads offsets")
plt.ylabel("Count")

img_name = filename.replace(".txt", ".png")
plt.savefig(img_name)





# Save new offsets and lengths and reads to new txt file
new_filename = filename.replace(".txt", "_coalesce.txt")
with open(new_filename, "w") as f:
    f.write("{:<15}".format("Segment"))
    f.write("{:<15}".format("Offset"))
    f.write("{:<15}\n".format("Length"))
    for i in range(len(nonconsec_reads)):
        f.write("{:<15}".format(i))
        f.write("{:<15}".format(nonconsec_reads[i][0]))
        f.write("{:<15}\n".format(nonconsec_reads[i][1]))
        
