#http://www.bogotobogo.com/python/python_matplotlib.php#Point_distribution
from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt
import numpy as np

fig = plt.figure()

#%% compute uniform samples on sphere
#The drawback is that there is always some patterns in the histogram
##http://blog.marmakoide.org/?p=1
#n = 2**20
#
#golden_angle = np.pi * (3 - np.sqrt(5))
#theta = golden_angle * np.arange(n)
#z = np.linspace(1 - 1.0 / n, 1.0 / n - 1, n)
#radius = np.sqrt(1 - z * z) 
#
#points = np.zeros((n, 3))
#points[:,0] = radius * np.cos(theta)
#points[:,1] = radius * np.sin(theta)
#points[:,2] = z


#%% compute the random samples
#http://www.bogotobogo.com/Algorithms/uniform_distribution_sphere.php
#http://en.wikibooks.org/wiki/Mathematica/Uniform_Spherical_Distribution
n = 2**24
sample_rate = 2**18;
v = np.random.random_integers(0,sample_rate,n) /sample_rate
phi = np.arccos(2 * v - 1)

u = np.random.random_integers(0,sample_rate - 1,n) /sample_rate
theta = 2 * np.pi * u

d = np.zeros((n, 3))
radius = 1
d[:,0] = radius * np.cos(theta)*np.sin(phi);
d[:,1] = radius * np.sin(theta)*np.sin(phi);
d[:,2] = radius * np.cos(phi);

#hist, bin_edges= np.histogram(theta, 10)

#print(hist)
#print(bin_edges)


#%% plot the sphere
#ax = fig.add_subplot(231, projection='3d')
#ax.set_aspect("equal")
#
#u = np.linspace(0, 2 * np.pi, 60)
#v = np.linspace(0, np.pi, 30)
#
#x = np.outer(np.cos(u), np.sin(v))
#y = np.outer(np.sin(u), np.sin(v))
#z = np.outer(np.ones(np.size(u)), np.cos(v))
#
#ax.plot_surface(x, y, z,  rstride=1, cstride=1, color='c', alpha = 1.0, linewidth = 0)
#plt.axis('off')
#ax.scatter(points[:,0], points[:,1], points[:,2], s = 1)

#################
#%%
#comment the following line for higher performance

d_nx = d[np.array(d[:,0] <= - abs(d[:,1])) & np.array(d[:,0] <= - abs(d[:,2]))]
d_px = d[np.array(d[:,0] > abs(d[:,1])) & np.array(d[:,0] > abs(d[:,2]))]
d_ny = d[np.array(d[:,1] < - abs(d[:,0])) & np.array(d[:,1] <= - abs(d[:,2]))]
d_py = d[np.array(d[:,1] >= abs(d[:,0])) & np.array(d[:,1] > abs(d[:,2]))]
d_nz = d[np.array(d[:,2] < - abs(d[:,0])) & np.array(d[:,2] < - abs(d[:,1]))]
d_pz = d[np.array(d[:,2] >= abs(d[:,0])) & np.array(d[:,2] >= abs(d[:,1]))]

total = len(d_px) + len(d_nx) + len(d_py) + len(d_ny) + len(d_pz) + len(d_nz)

texCoords_nx = np.zeros((len(d_nx), 2))
texCoords_nx[:, 0] = d_nx[:,1] / d_nx[:,0]
texCoords_nx[:, 1] = d_nx[:,2] / d_nx[:,0]

texCoords_px = np.zeros((len(d_px), 2))
texCoords_px[:, 0] = d_px[:,1] / d_px[:,0]
texCoords_px[:, 1] = d_px[:,2] / d_px[:,0]

texCoords_ny = np.zeros((len(d_ny), 2))
texCoords_ny[:, 0] = d_ny[:,2] / d_ny[:,1]
texCoords_ny[:, 1] = d_ny[:,0] / d_ny[:,1]

texCoords_py = np.zeros((len(d_py), 2))
texCoords_py[:, 0] = d_py[:,2] / d_py[:,1]
texCoords_py[:, 1] = d_py[:,0] / d_py[:,1]

texCoords_nz = np.zeros((len(d_nz), 2))
texCoords_nz[:, 0] = d_nz[:,0] / d_nz[:,2]
texCoords_nz[:, 1] = d_nz[:,1] / d_nz[:,2]

texCoords_pz = np.zeros((len(d_pz), 2))
texCoords_pz[:, 0] = d_pz[:,0] / d_pz[:,2]
texCoords_pz[:, 1] = d_pz[:,1] / d_pz[:,2]

#f1 = fig.add_subplot(232)
#f1.scatter(texCoords_px[:,0], texCoords_px[:,1], s = 1)
#f1.set_aspect("equal")
nbin = 20

H_nx, xedges, yedges = np.histogram2d(texCoords_nx[:,0], texCoords_nx[:,1], nbin, range=[[-1,1],[-1,1]])
f1 = fig.add_subplot(321)
imgplot1 = f1.imshow(H_nx)
plt.colorbar(imgplot1)

H_px, xedges, yedges = np.histogram2d(texCoords_px[:,0], texCoords_px[:,1], nbin, range=[[-1,1],[-1,1]])
f2 = fig.add_subplot(322)
imgplot2 = f2.imshow(H_px)
plt.colorbar(imgplot2)

H_ny, xedges, yedges = np.histogram2d(texCoords_ny[:,0], texCoords_ny[:,1], nbin, range=[[-1,1],[-1,1]])
f3 = fig.add_subplot(323)
imgplot3 = f3.imshow(H_ny)
plt.colorbar(imgplot3)

H_py, xedges, yedges = np.histogram2d(texCoords_py[:,0], texCoords_py[:,1], nbin, range=[[-1,1],[-1,1]])
f4 = fig.add_subplot(324)
imgplot4 = f4.imshow(H_py)
plt.colorbar(imgplot4)

H_nz, xedges, yedges = np.histogram2d(texCoords_nz[:,0], texCoords_nz[:,1], nbin, range=[[-1,1],[-1,1]])
f5 = fig.add_subplot(325)
imgplot5 = f5.imshow(H_nz)
plt.colorbar(imgplot5)

H_pz, xedges, yedges = np.histogram2d(texCoords_pz[:,0], texCoords_pz[:,1], nbin, range=[[-1,1],[-1,1]])
f6 = fig.add_subplot(326)
imgplot6 = f6.imshow(H_pz)
plt.colorbar(imgplot6)



#%% 
#compute the solid angle for each patch and 
#scale the histogram by the solid angle to get the sample density
delta = 2.0 / nbin
solAng = np.zeros((nbin, nbin))

for i in range(nbin):
    for j in range(nbin):
        a1 = 2 - 2 * i * delta
        a2 = 2 - 2 * j * delta
        sa = 4 * np.arcsin(np.sin(np.arctan(a1 * 0.5)) * np.sin(np.arctan(a2 * 0.5)))
        
        b1 = 2 - 2 * i * delta - 2 * delta
        b2 = 2 - 2 * j * delta
        sb = 4 * np.arcsin(np.sin(np.arctan(b1 * 0.5)) * np.sin(np.arctan(b2 * 0.5)))

        c1 = 2 - 2 * i * delta
        c2 = 2 - 2 * j * delta - 2 * delta
        sc = 4 * np.arcsin(np.sin(np.arctan(c1 * 0.5)) * np.sin(np.arctan(c2 * 0.5)))

        d1 = 2 - 2 * i * delta - 2 * delta
        d2 = 2 - 2 * j * delta - 2 * delta
        sd = 4 * np.arcsin(np.sin(np.arctan(d1 * 0.5)) * np.sin(np.arctan(d2 * 0.5)))
        
        solAng[i, j] = (sa - sb - sc + sd) * 0.25
        
sumSolAng = sum(sum(solAng))
print(sumSolAng)

den_nx = H_nx / solAng
den_px = H_px / solAng
den_ny = H_ny / solAng
den_py = H_py / solAng
den_nz = H_nz / solAng
den_pz = H_pz / solAng

den_uniform = n / (4 * np.pi)
print(den_uniform)

#%% plot the scaled density

fig2 = plt.figure()

f1 = fig2.add_subplot(321)
imgplot1 = f1.imshow(den_nx / den_uniform, vmin = 0.9, vmax = 1.1)
plt.colorbar(imgplot1)

f2 = fig2.add_subplot(322)
imgplot2 = f2.imshow(den_px / den_uniform, vmin = 0.9, vmax = 1.1)
plt.colorbar(imgplot2)

f3 = fig2.add_subplot(323)
imgplot3 = f3.imshow(den_ny / den_uniform, vmin = 0.9, vmax = 1.1)
plt.colorbar(imgplot3)

f4 = fig2.add_subplot(324)
imgplot4 = f4.imshow(den_py / den_uniform, vmin = 0.9, vmax = 1.1)
plt.colorbar(imgplot4)

f5 = fig2.add_subplot(325)
imgplot5 = f5.imshow(den_nz / den_uniform, vmin = 0.9, vmax = 1.1)
plt.colorbar(imgplot5)

f6 = fig2.add_subplot(326)
imgplot6 = f6.imshow(den_pz / den_uniform, vmin = 0.9, vmax = 1.1)
plt.colorbar(imgplot6)

plt.show()

#https://www.physicsforums.com/threads/area-of-a-spherical-patch.505467/