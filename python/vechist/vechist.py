#%%
import numpy as np
import read_vec
import cubemap_coords as cube
import plot_vec as pv
import matplotlib.pyplot as plt


#d = sample_ball.sample(2**8)
#d = sample_uniform.sample(2**20)
#d = sample_random.sample(2**24)
#d = read_vec.read("D:/data/isabel/UVWf01.vec")
d = read_vec.read("D:/data/plume/15plume3d421.vec")
#d = read_vec.read("D:/data/sample/test3.vec")
dim = d.shape
d = np.reshape(d, (dim[2], dim[1], dim[0], 3))
step = 4
print(d.shape)
d = d[::step, ::step, ::step,:]
dim = d.shape
print(d.shape)
#pv.plot(d, 4)

d_idx = np.zeros(d.shape[0] * d.shape[1] * d.shape[2], dtype=np.float32)
d = np.reshape(d, (d.shape[0] * d.shape[1] * d.shape[2], 3))

print(d_idx.shape)

cubemap_size = 32
d_idx = np.apply_along_axis( cube.get, axis=1, arr=d, size=cubemap_size)
#print(d_idx)
d_idx = np.reshape(d_idx, (dim[2], dim[1], dim[0]), order = 'F')
#root = cube.TreeNode()
centers = []
vectors = []
cube.Split(centers, vectors, d_idx, cubemap_size, np.array([0, 0, 0]))

#allcenters = cube.inorderTraversal(root)
centers_array = np.array(centers)
vectors_array = np.array(vectors)

#import numpy as np
from mpl_toolkits.mplot3d import Axes3D
#import matplotlib.pyplot as plt

fig = plt.figure()    
ax = fig.gca(projection='3d')
ax.set_aspect('equal')
#print(vectors[:, 2])
print(centers_array.shape)
print(type(centers_array[0]))
ax.quiver(centers_array[:, 0], centers_array[:, 1], centers_array[:, 2], vectors_array[:, 0], vectors_array[:, 1], vectors_array[:, 2], linewidths=(20,))

plt.show()


#print('finished generating samples...')

#################
#%%
#comment the following line for higher performance

# there are too many points on the boundary that is not on either side.
#d_nx = d[np.array(d[:,0] <= - abs(d[:,1])) & np.array(d[:,0] <= - abs(d[:,2]))]
#d_px = d[np.array(d[:,0] > abs(d[:,1])) & np.array(d[:,0] > abs(d[:,2]))]
#d_ny = d[np.array(d[:,1] < - abs(d[:,0])) & np.array(d[:,1] <= - abs(d[:,2]))]
#d_py = d[np.array(d[:,1] >= abs(d[:,0])) & np.array(d[:,1] > abs(d[:,2]))]
#d_nz = d[np.array(d[:,2] < - abs(d[:,0])) & np.array(d[:,2] < - abs(d[:,1]))]
#d_pz = d[np.array(d[:,2] >= abs(d[:,0])) & np.array(d[:,2] >= abs(d[:,1]))]

            

#%%
#
#
#d_idx = np.zeros(d.shape)
#
#d_nx = d[np.array(d[:,0] < - abs(d[:,1])) & np.array(d[:,0] < - abs(d[:,2]))]
#d_px = d[np.array(d[:,0] > abs(d[:,1])) & np.array(d[:,0] > abs(d[:,2]))]
#d_ny = d[np.array(d[:,1] < - abs(d[:,0])) & np.array(d[:,1] < - abs(d[:,2]))]
#d_py = d[np.array(d[:,1] > abs(d[:,0])) & np.array(d[:,1] > abs(d[:,2]))]
#d_nz = d[np.array(d[:,2] < - abs(d[:,0])) & np.array(d[:,2] < - abs(d[:,1]))]
#d_pz = d[np.array(d[:,2] > abs(d[:,0])) & np.array(d[:,2] > abs(d[:,1]))]
#
#total = len(d_px) + len(d_nx) + len(d_py) + len(d_ny) + len(d_pz) + len(d_nz)
#
#texCoords_nx = np.zeros((len(d_nx), 2))
#texCoords_nx[:, 0] = d_nx[:,1] / d_nx[:,0]
#texCoords_nx[:, 1] = d_nx[:,2] / d_nx[:,0]
#
#texCoords_px = np.zeros((len(d_px), 2))
#texCoords_px[:, 0] = d_px[:,1] / d_px[:,0]
#texCoords_px[:, 1] = d_px[:,2] / d_px[:,0]
#
#texCoords_ny = np.zeros((len(d_ny), 2))
#texCoords_ny[:, 0] = d_ny[:,2] / d_ny[:,1]
#texCoords_ny[:, 1] = d_ny[:,0] / d_ny[:,1]
#
#texCoords_py = np.zeros((len(d_py), 2))
#texCoords_py[:, 0] = d_py[:,2] / d_py[:,1]
#texCoords_py[:, 1] = d_py[:,0] / d_py[:,1]
#
#texCoords_nz = np.zeros((len(d_nz), 2))
#texCoords_nz[:, 0] = d_nz[:,0] / d_nz[:,2]
#texCoords_nz[:, 1] = d_nz[:,1] / d_nz[:,2]
#
#texCoords_pz = np.zeros((len(d_pz), 2))
#texCoords_pz[:, 0] = d_pz[:,0] / d_pz[:,2]
#texCoords_pz[:, 1] = d_pz[:,1] / d_pz[:,2]
#
##f1 = fig.add_subplot(232)
##f1.scatter(texCoords_px[:,0], texCoords_px[:,1], s = 1)
##f1.set_aspect("equal")
#nbin = 64
#
#H_nx, xedges, yedges = np.histogram2d(texCoords_nx[:,0], texCoords_nx[:,1], nbin, range=[[-1,1],[-1,1]])
#f1 = fig.add_subplot(321)
#imgplot1 = f1.imshow(H_nx)
#plt.colorbar(imgplot1)
#
#H_px, xedges, yedges = np.histogram2d(texCoords_px[:,0], texCoords_px[:,1], nbin, range=[[-1,1],[-1,1]])
#f2 = fig.add_subplot(322)
#imgplot2 = f2.imshow(H_px)
#plt.colorbar(imgplot2)
#
#H_ny, xedges, yedges = np.histogram2d(texCoords_ny[:,0], texCoords_ny[:,1], nbin, range=[[-1,1],[-1,1]])
#f3 = fig.add_subplot(323)
#imgplot3 = f3.imshow(H_ny)
#plt.colorbar(imgplot3)
#
#H_py, xedges, yedges = np.histogram2d(texCoords_py[:,0], texCoords_py[:,1], nbin, range=[[-1,1],[-1,1]])
#f4 = fig.add_subplot(324)
#imgplot4 = f4.imshow(H_py)
#plt.colorbar(imgplot4)
#
#H_nz, xedges, yedges = np.histogram2d(texCoords_nz[:,0], texCoords_nz[:,1], nbin, range=[[-1,1],[-1,1]])
#f5 = fig.add_subplot(325)
#imgplot5 = f5.imshow(H_nz)
#plt.colorbar(imgplot5)
#
#H_pz, xedges, yedges = np.histogram2d(texCoords_pz[:,0], texCoords_pz[:,1], nbin, range=[[-1,1],[-1,1]])
#f6 = fig.add_subplot(326)
#imgplot6 = f6.imshow(H_pz)
#plt.colorbar(imgplot6)
#
#
#
###%% 
###compute the solid angle for each patch and 
###scale the histogram by the solid angle to get the sample density
##delta = 2.0 / nbin
##solAng = np.zeros((nbin, nbin))
##
##for i in range(nbin):
##    for j in range(nbin):
##        a1 = 2 - 2 * i * delta
##        a2 = 2 - 2 * j * delta
##        sa = 4 * np.arcsin(np.sin(np.arctan(a1 * 0.5)) * np.sin(np.arctan(a2 * 0.5)))
##        
##        b1 = 2 - 2 * i * delta - 2 * delta
##        b2 = 2 - 2 * j * delta
##        sb = 4 * np.arcsin(np.sin(np.arctan(b1 * 0.5)) * np.sin(np.arctan(b2 * 0.5)))
##
##        c1 = 2 - 2 * i * delta
##        c2 = 2 - 2 * j * delta - 2 * delta
##        sc = 4 * np.arcsin(np.sin(np.arctan(c1 * 0.5)) * np.sin(np.arctan(c2 * 0.5)))
##
##        d1 = 2 - 2 * i * delta - 2 * delta
##        d2 = 2 - 2 * j * delta - 2 * delta
##        sd = 4 * np.arcsin(np.sin(np.arctan(d1 * 0.5)) * np.sin(np.arctan(d2 * 0.5)))
##        
##        solAng[i, j] = (sa - sb - sc + sd) * 0.25
##        
##sumSolAng = sum(sum(solAng))
##print(sumSolAng)
##
##den_nx = H_nx / solAng
##den_px = H_px / solAng
##den_ny = H_ny / solAng
##den_py = H_py / solAng
##den_nz = H_nz / solAng
##den_pz = H_pz / solAng
##
##den_uniform = total / (4 * np.pi)
###print(den_nx / den_uniform)
##
###%% plot the scaled density
##
##fig2 = plt.figure()
##
##f1 = fig2.add_subplot(321)
##imgplot1 = f1.imshow(den_nx / den_uniform)#, vmin = 0.7, vmax = 1.3)
##plt.colorbar(imgplot1)
##
##f2 = fig2.add_subplot(322)
##imgplot2 = f2.imshow(den_px / den_uniform)#, vmin = 0.7, vmax = 1.3)
##plt.colorbar(imgplot2)
##
##f3 = fig2.add_subplot(323)
##imgplot3 = f3.imshow(den_ny / den_uniform)#, vmin = 0.7, vmax = 1.3)
##plt.colorbar(imgplot3)
##
##f4 = fig2.add_subplot(324)
##imgplot4 = f4.imshow(den_py / den_uniform)#, vmin = 0.7, vmax = 1.3)
##plt.colorbar(imgplot4)
##
##f5 = fig2.add_subplot(325)
##imgplot5 = f5.imshow(den_nz / den_uniform)#, vmin = 0.7, vmax = 1.3)
##plt.colorbar(imgplot5)
##
##f6 = fig2.add_subplot(326)
##imgplot6 = f6.imshow(den_pz / den_uniform)#, vmin = 0.7, vmax = 1.3)
##plt.colorbar(imgplot6)
##
##==============================================================================
#plt.show()
##==============================================================================
#
##https://www.physicsforums.com/threads/area-of-a-spherical-patch.505467/