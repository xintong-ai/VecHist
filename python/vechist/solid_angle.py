#%%
import numpy as np
import cubemap_coords as cube
import plot_vec as pv
import matplotlib.pyplot as plt
import time
import math
import sample_random_ball

#Chris Jacobsen -
#This computes Super Quadrics for the Dark Sky data
#It is being commented out for now, until we get the Dark Sky data working or create a different flow for it
#cube.loadHaloComputeSuperquadric()
#pause

#time_start = time.time()

d = sample_random_ball.sample(2**24, 2**8)
#d = sample_uniform.sample(2**20)
#d = sample_random.sample(2**24)
#d = read_vec.read("D:/data/isabel/UVWf01.vec")
#d = read_vec.read("D:/data/plume/15plume3d421.vec")
#d = read_vec.read("D:/data/sample/test3.vec")

cubemap_size = 32
d_idx = np.apply_along_axis( cube.get, axis=1, arr=d, size=cubemap_size)

cube_hist = cube.GenCubemap(d_idx.ravel(), cubemap_size)
plt.imshow(cube_hist[1:(cubemap_size - 1), 1:(cubemap_size - 1)])
plt.colorbar()

frame1 = plt.gca()
frame1.axes.get_xaxis().set_ticks([])
frame1.axes.get_yaxis().set_ticks([])
#plt.show()
plt.savefig("pic.png")