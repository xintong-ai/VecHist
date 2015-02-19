# -*- coding: utf-8 -*-
"""
Created on Thu Feb 19 01:04:40 2015

@author: tong
"""

def plot(d, stride):
    import numpy as np
    from mpl_toolkits.mplot3d import Axes3D
    import matplotlib.pyplot as plt

    fig = plt.figure()    
    ax = fig.gca(projection='3d')
    ax.set_aspect('equal')
    
    d = d[::stride,::stride,::stride,:]
    y, z, x = np.meshgrid(np.arange(0, d.shape[1]), np.arange(0, d.shape[0]), np.arange(0, d.shape[2]))
    
#    print(d[:,:,:,0].shape)
#    print(x.shape)
#    print(y.shape)
#    print(z.shape)

    ax.quiver(x, y, z, d[:,:,:,0], d[:,:,:,1], d[:,:,:,2])
    
    plt.show()