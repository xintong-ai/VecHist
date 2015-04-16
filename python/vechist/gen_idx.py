#%%
import numpy as np
import read_vec
import cubemap_coords as cube
import plot_vec as pv
import matplotlib.pyplot as plt
import time

print ('Starting the gen ids script\n')
time_start = time.time()

#d = sample_ball.sample(2**8)
#d = sample_uniform.sample(2**20)
#d = sample_random.sample(2**24)
#d = read_vec.read("D:/data/isabel/UVWf01.vec")
d = read_vec.read("C:\\Users\\datahead8888\\Documents\\sciVis\\data\\15plume3d421.vec")
#d = read_vec.read("D:/data/sample/test3.vec")
#d = read_vec.read("D:/data/plume/15plume3d421.vec")
#dim = d.shape
#d = np.reshape(d, (dim[2], dim[1], dim[0], 3))
step = 1
#print(d.shape)
d = d[::step, ::step, ::step,:]
dim = d.shape
#print(d.shape)
#pv.plot(d, 16)

d_idx = np.zeros(d.shape[0] * d.shape[1] * d.shape[2], dtype=np.float32)
d = np.reshape(d, (d.shape[0] * d.shape[1] * d.shape[2], 3))

#print(d_idx.shape)

cubemap_size = 16
d_idx = np.apply_along_axis( cube.get, axis=1, arr=d, size=cubemap_size)
#print(d_idx)
d_idx = np.reshape(d_idx, (dim[2], dim[1], dim[0]), order = 'F')
np.save("d_idx", d_idx)
