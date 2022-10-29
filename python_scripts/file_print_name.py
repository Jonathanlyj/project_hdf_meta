import h5py
import time

def visitor_func(name, node):
    print(name)

f = h5py.File("./train64.h5",'r')
propid = h5py.h5p.create(h5py.h5p.FILE_ACCESS)


print(propid.get_meta_block_size())
propid.set_meta_block_size(1024)

start_time = time.time()
f.visititems(visitor_func)
print("--- %s seconds ---" % (time.time() - start_time))
f.close()
