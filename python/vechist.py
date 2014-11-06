#http://www.bogotobogo.com/python/python_matplotlib.php#Point_distribution
from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt
import numpy as np

fig = plt.figure()
ax = fig.add_subplot(231, projection='3d')
ax.set_aspect("equal")

# top: elev=90, side: elev=0
#ax.view_init(elev=0, azim=0)

u = np.linspace(0, 2 * np.pi, 60)
v = np.linspace(0, np.pi, 30)

x = np.outer(np.cos(u), np.sin(v))
y = np.outer(np.sin(u), np.sin(v))
z = np.outer(np.ones(np.size(u)), np.cos(v))

#ax.plot_surface(x, y, z,  rstride=2, cstride=2, color='b', #alpha = 0.3, linewidth = 0, cmap=cm.jet)
ax.plot_surface(x, y, z,  rstride=1, cstride=1, color='c', alpha = 1.0, linewidth = 0)
plt.axis('off')

#http://blog.marmakoide.org/?p=1
n = 2**8

golden_angle = np.pi * (3 - np.sqrt(5))
theta = golden_angle * np.arange(n)
z = np.linspace(1 - 1.0 / n, 1.0 / n - 1, n)
radius = np.sqrt(1 - z * z) 

points = np.zeros((n, 3))
points[:,0] = radius * np.cos(theta)
points[:,1] = radius * np.sin(theta)
points[:,2] = z

#comment the following line for higher performance
ax.scatter(points[:,0], points[:,1], points[:,2], s = 1)

points_px = points[np.array(points[:,0] >= abs(points[:,1])) & np.array(points[:,0] >= abs(points[:,2]))]

bx = fig.add_subplot(232)
bx.scatter(points_px[:,1], points_px[:,2], s = 1)
bx.set_aspect("equal")

points_angle = np.zeros((len(points_px), 2))
points_angle[:, 0] = points_px[:,1] / points_px[:,0]
points_angle[:, 1] = points_px[:,2] / points_px[:,0]

cx = fig.add_subplot(233)
cx.scatter(points_angle[:,0], points_angle[:,1], s = 1)
cx.set_aspect("equal")

nbin = 16
H, xedges, yedges = np.histogram2d(points_angle[:,0], points_angle[:,1], nbin)
dx = fig.add_subplot(234)
#print(H)
imgplot1 = dx.imshow(H)
plt.colorbar(imgplot1)

delta = 2.0 / nbin
factors = np.zeros((len(H), len(H)))

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
        
        factors[i, j] = (sa - sb - sc + sd) * 0.25
        
        #factors[i, j] = (np.tan(ang1 + delta) - np.tan(ang1)) * (np.tan(ang2 + delta) - np.tan(ang2)) / 3

H_avg = H / factors

ex = fig.add_subplot(235)
imgplot2 = ex.imshow(factors)
plt.colorbar(imgplot2)

fx = fig.add_subplot(236)
imgplot3 = fx.imshow(H_avg)
plt.colorbar(imgplot3)

plt.show()

#print(factors)
#https://www.physicsforums.com/threads/area-of-a-spherical-patch.505467/