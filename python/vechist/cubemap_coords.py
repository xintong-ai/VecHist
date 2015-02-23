# -*- coding: utf-8 -*-
"""
Created on Sun Dec 14 23:42:54 2014

@author: tong
"""
import numpy as np
import math
import matplotlib.pyplot as plt

def ShowHist(hist):
    size = hist.shape[1]
#    print(np.sum(hist))
        
#    max_val = size * size * 6 - 1
    max_val = np.max(hist)

    plt_idx = 1
    plt.subplot(345)
    plt.imshow(hist[plt_idx*size:(plt_idx+1)*size, :].transpose(), aspect='equal', interpolation='nearest', vmin=0, vmax=max_val)
    plt.gca().invert_yaxis()
    plt.colorbar()

    plt_idx = 4
    plt.subplot(346)
    plt.imshow(hist[plt_idx*size:(plt_idx+1)*size, :].transpose(), aspect='equal', interpolation='nearest', vmin=0, vmax=max_val)
    plt.gca().invert_yaxis()
    plt.colorbar()

    plt_idx = 0
    plt.subplot(347)
    plt.imshow(hist[plt_idx*size:(plt_idx+1)*size, :].transpose(), aspect='equal', interpolation='nearest', vmin=0, vmax=max_val)
    plt.gca().invert_yaxis()
    plt.colorbar()
    
    plt_idx = 5
    plt.subplot(348)
    plt.imshow(hist[plt_idx*size:(plt_idx+1)*size, :].transpose(), aspect='equal', interpolation='nearest', vmin=0, vmax=max_val)
    plt.gca().invert_yaxis()
    plt.colorbar()
    
    #http://www.humus.name/index.php?page=Textures
    #3 is above, and 2 is below, because the y coordinates start from up.
    plt_idx = 3
    plt.subplot(342)
    plt.imshow(hist[plt_idx*size:(plt_idx+1)*size, :].transpose(), aspect='equal', interpolation='nearest', vmin=0, vmax=max_val)
    plt.gca().invert_yaxis()
    plt.colorbar()

    plt_idx = 2
    plt.subplot(3,4,10)
    plt.imshow(hist[plt_idx*size:(plt_idx+1)*size, :].transpose(), aspect='equal', interpolation='nearest', vmin=0, vmax=max_val)
    plt.gca().invert_yaxis()
    plt.colorbar()
    
    plt.show()


def FindPeak(d):
    
    return
    
def SolidAngles(nbin):
    #compute the solid angle for each patch and 
    #scale the histogram by the solid angle to get the sample density
    delta = 2.0 / nbin
    solAng = np.zeros((nbin, nbin))
    
    for i in range(nbin):
        for j in range(nbin):
            a1 = 1 - i * delta
            a2 = 1 - j * delta
            sa = 4 * np.arcsin(np.sin(np.arctan(a1)) * np.sin(np.arctan(a2)))
            
            b1 = 1 - (i + 1) * delta
            b2 = 1 - j * delta
            sb = 4 * np.arcsin(np.sin(np.arctan(b1)) * np.sin(np.arctan(b2)))
    
            c1 = 1 - i * delta
            c2 = 1 - (j + 1) * delta
            sc = 4 * np.arcsin(np.sin(np.arctan(c1)) * np.sin(np.arctan(c2)))
    
            d1 = 1 - (i + 1) * delta
            d2 = 1 - (j + 1) * delta
            sd = 4 * np.arcsin(np.sin(np.arctan(d1)) * np.sin(np.arctan(d2)))
            
            solAng[i, j] = (sa - sb - sc + sd) * 0.25
    
    return solAng
            
#    sumSolAng = sum(sum(solAng))
#    print(sumSolAng)
#    
#    den_nx = H_nx / solAng
#    den_px = H_px / solAng
#    den_ny = H_ny / solAng
#    den_py = H_py / solAng
#    den_nz = H_nz / solAng
#    den_pz = H_pz / solAng
#    
#    den_uniform = total / (4 * np.pi)
    #print(den_nx / den_uniform)

    

def GenCubemap(d, size):
    [hist, bin_edges] = np.histogram(d, np.arange(0, size * size * 6 + 1)) 
#    hist = np.arange(0, size * size * 6)       
#    print(hist.shape)
    hist = np.reshape(hist, (6 * size, size))
#    hist = np.ones((6 * size, size))
    
    #normalize by solid angles
    sol_ang = SolidAngles(size)
    for i in range(0, 6):
        hist[i*size:(i+1)*size, :] = hist[i*size:(i+1)*size, :] / sol_ang
    
    #normalize to be a histogram
    hist_sum = np.sum(hist)
    hist = hist / hist_sum
#    ShowHist(hist)
    
    return hist


   
def get(v, size):
#    print("size:"+str(size))
    coef = size * 0.5
    if    v[2] < - abs(v[0])  and v[2] <= - abs(v[1]):
        idx3d = [5, math.floor((-v[0] / abs(v[2]) + 1) * coef) , math.floor((-v[1] / abs(v[2]) + 1) * coef)]
    elif    v[0] < - abs(v[1])  and v[0] <= - abs(v[2]):
        idx3d = [1, math.floor((-v[1] / abs(v[0]) + 1) * coef) , math.floor((v[2] / abs(v[0]) + 1) * coef)]
    elif    v[1] <= - abs(v[2])  and v[1] < - abs(v[0]):
        idx3d = [3, math.floor((v[2] / abs(v[1]) + 1) * coef) , math.floor((v[0] / abs(v[1]) + 1) * coef)]
    elif    v[2] >= abs(v[0]) and v[2] > abs(v[1]):
        idx3d = [4, math.floor((v[0] / abs(v[2]) + 1) * coef) , math.floor((v[1] / abs(v[2]) + 1) * coef)]
    elif    v[0] >= abs(v[1])    and v[0] > abs(v[2]):
        idx3d = [0, math.floor((v[1] / abs(v[0]) + 1) * coef) , math.floor((-v[2] / abs(v[0]) + 1) * coef)]
    elif    v[1] > abs(v[2])    and v[1] >= abs(v[0]):
        idx3d = [2, math.floor((-v[2] / abs(v[1]) + 1) * coef) , math.floor((-v[0] / abs(v[1]) + 1) * coef)]
    else:
        idx3d = [0,0,0]
        
    return idx3d[0] * size * size + idx3d[1] * size + idx3d[2]
        
#    d_nx = d[np.array(d[:,0] < - abs(d[:,1])) & np.array(d[:,0] < - abs(d[:,2]))]
#    d_px = d[np.array(d[:,0] > abs(d[:,1])) & np.array(d[:,0] > abs(d[:,2]))]
#    d_ny = d[np.array(d[:,1] < - abs(d[:,0])) & np.array(d[:,1] < - abs(d[:,2]))]
#    d_py = d[np.array(d[:,1] > abs(d[:,0])) & np.array(d[:,1] > abs(d[:,2]))]
#    d_nz = d[np.array(d[:,2] < - abs(d[:,0])) & np.array(d[:,2] < - abs(d[:,1]))]
#    d_pz = d[np.array(d[:,2] > abs(d[:,0])) & np.array(d[:,2] > abs(d[:,1]))]
#    
#    total = len(d_px) + len(d_nx) + len(d_py) + len(d_ny) + len(d_pz) + len(d_nz)
#    
#    texCoords_nx = np.zeros((len(d_nx), 2))
#    texCoords_nx[:, 0] = d_nx[:,1] / d_nx[:,0]
#    texCoords_nx[:, 1] = d_nx[:,2] / d_nx[:,0]
#    
#    texCoords_px = np.zeros((len(d_px), 2))
#    texCoords_px[:, 0] = d_px[:,1] / d_px[:,0]
#    texCoords_px[:, 1] = d_px[:,2] / d_px[:,0]
#    
#    texCoords_ny = np.zeros((len(d_ny), 2))
#    texCoords_ny[:, 0] = d_ny[:,2] / d_ny[:,1]
#    texCoords_ny[:, 1] = d_ny[:,0] / d_ny[:,1]
#    
#    texCoords_py = np.zeros((len(d_py), 2))
#    texCoords_py[:, 0] = d_py[:,2] / d_py[:,1]
#    texCoords_py[:, 1] = d_py[:,0] / d_py[:,1]
#    
#    texCoords_nz = np.zeros((len(d_nz), 2))
#    texCoords_nz[:, 0] = d_nz[:,0] / d_nz[:,2]
#    texCoords_nz[:, 1] = d_nz[:,1] / d_nz[:,2]
#    
#    texCoords_pz = np.zeros((len(d_pz), 2))
#    texCoords_pz[:, 0] = d_pz[:,0] / d_pz[:,2]
#    texCoords_pz[:, 1] = d_pz[:,1] / d_pz[:,2]

    
                