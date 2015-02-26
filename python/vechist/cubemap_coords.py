# -*- coding: utf-8 -*-
"""
Created on Sun Dec 14 23:42:54 2014

@author: tong
"""
import numpy as np
import math
import matplotlib.pyplot as plt
import copy
from scipy import asarray as ar,exp
from scipy.optimize import curve_fit
#from mayavi import mlab



def PlotMask(mask):
    dim = mask.shape
    print(np.count_nonzero(mask))
    plt.figure()
    plt.imshow(mask[math.floor(dim[0]*0.5), :, :])

def SetMask(idx, mask, cand):
    dim = idx.shape
    for k in range(dim[2]):
        for j in range(dim[1]):
            for i in range(dim[0]):
                v = idx[i, j, k]
                if(v in cand):
                    mask[i, j, k] = 1
                else:
                    mask[i, j, k] = 0

def FindTowards(ret, hist, _pos, _f, _dirx, _diry, _step, _step2, threshold):
    pos = copy.deepcopy(_pos)
    f = copy.deepcopy(_f)
    dirx = copy.deepcopy(_dirx)
    diry = copy.deepcopy(_diry)
    step = copy.deepcopy(_step)
    step2 = copy.deepcopy(_step2)
    
    size = hist.shape[1]
    idx = -1
#    ret = []

    if (hist[pos[0] + f * size, pos[1]] > threshold):
        idx = (pos[0] + f * size) * size + pos[1]
        
    if(idx != -1):
        ret.append(idx)        
        #make a turn
        if(step2!=0):
            if(dirx != 0):
                FindTowards(ret, hist, pos, f, 0, -1, step2, 0, threshold)
                FindTowards(ret, hist, pos, f, 0,  1, step2, 0, threshold)
            elif(diry != 0):
                FindTowards(ret, hist, pos, f, -1, 0, step2, 0, threshold)
                FindTowards(ret, hist, pos, f,  1, 0, step2, 0, threshold)
    else:
        return
    
    #march
    if(dirx != 0):
        pos[0] += dirx
        if (pos[0] >= size):
            pos[0] = pos[0] % size
            pos[0], pos[1] = pos[1], pos[0]
            f = (f + 1) % 6
            dirx = 0
            diry = 1
        elif(pos[0] < 0):
            pos[0] = pos[0] % size
            pos[0], pos[1] = pos[1], pos[0]
            f = (f + 4) % 6    
            dirx = 0
            diry = -1
            pos[0] = (- 1 - pos[0]) % size            
    elif(diry != 0):
        pos[1] += diry
        if(pos[1] >= size):
            pos[1] = pos[1] % size
            pos[0], pos[1] = pos[1], pos[0]
            f = (f + 2) % 6
            dirx = 1
            diry = 0
            pos[1] = (- 1 - pos[1]) % size
        elif(pos[1] < 0):
            pos[1] = pos[1] % size
            pos[0], pos[1] = pos[1], pos[0]
            f = (f + 5) % 6
            dirx = -1
            diry = 0

    if(step != 0):
        FindTowards(ret, hist, pos, f, dirx, diry, step - 1, step2, threshold)

    return
    

def FindPeak(hist):
    size = hist.shape[1]
    max_idx_1d = np.argmax(hist)
#    print(max_idx)
    max_idx = (math.floor(max_idx_1d / size), max_idx_1d % size)
    threshold = hist[max_idx] * 0.08
    nSteps = math.floor(size * 0.2)
    list1 = []
    list2 = []
    #TODO:duplicated results are found. Need to improve the algorithm later
    FindTowards(list1, hist, [max_idx[0] % size, max_idx[1]], math.floor(max_idx[0] / size), -1, 0, nSteps + 1, nSteps, threshold)
    FindTowards(list2, hist, [max_idx[0] % size, max_idx[1]], math.floor(max_idx[0] / size), 1, 0, nSteps + 1, nSteps, threshold)
#    arr = np.array(list1 + list2)    
    arr = np.array(list(set(list1+list2)))
    print(arr)

    x = np.floor(arr / size).astype(int)
    y = arr % size
#    print(x)
#    print(type(x))
#    print(y)
    plt.scatter(x, y, s = 2, color='white')
    #seeds = list1.append()
#    print(list1)
#    hist.tofile('hist.dat', sep=" ", format="%s")
    
#    print(max_idx)
#    print(hist[max_idx])
#    print(np.max(hist))
    
    return arr


def ShowHist(hist):
    size = hist.shape[1]
#    print(np.sum(hist))
        
#    max_val = size * size * 6 - 1
    max_val = np.max(hist)
    
    plt.imshow(hist.transpose(), aspect='equal', interpolation='nearest', vmin=0, vmax=max_val)
    plt.colorbar()
#    plt_idx = 1
#    plt.subplot(345)
#    plt.imshow(hist[plt_idx*size:(plt_idx+1)*size, :].transpose(), aspect='equal', interpolation='nearest', vmin=0, vmax=max_val)
#    plt.gca().invert_yaxis()
#    plt.colorbar()
#
#    plt_idx = 4
#    plt.subplot(346)
#    plt.imshow(hist[plt_idx*size:(plt_idx+1)*size, :].transpose(), aspect='equal', interpolation='nearest', vmin=0, vmax=max_val)
#    plt.gca().invert_yaxis()
#    plt.colorbar()
#
#    plt_idx = 0
#    plt.subplot(347)
#    plt.imshow(hist[plt_idx*size:(plt_idx+1)*size, :].transpose(), aspect='equal', interpolation='nearest', vmin=0, vmax=max_val)
#    plt.gca().invert_yaxis()
#    plt.colorbar()
#    
#    plt_idx = 5
#    plt.subplot(348)
#    plt.imshow(hist[plt_idx*size:(plt_idx+1)*size, :].transpose(), aspect='equal', interpolation='nearest', vmin=0, vmax=max_val)
#    plt.gca().invert_yaxis()
#    plt.colorbar()
#    
#    #http://www.humus.name/index.php?page=Textures
#    #3 is above, and 2 is below, because the y coordinates start from up.
#    plt_idx = 3
#    plt.subplot(342)
#    plt.imshow(hist[plt_idx*size:(plt_idx+1)*size, :].transpose(), aspect='equal', interpolation='nearest', vmin=0, vmax=max_val)
#    plt.gca().invert_yaxis()
#    plt.colorbar()
#
#    plt_idx = 2
#    plt.subplot(3,4,10)
#    plt.imshow(hist[plt_idx*size:(plt_idx+1)*size, :].transpose(), aspect='equal', interpolation='nearest', vmin=0, vmax=max_val)
#    plt.gca().invert_yaxis()
#    plt.colorbar()
    
    


    
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
        idx3d = [0, math.floor((-v[0] / abs(v[2]) + 1) * coef) , math.floor((-v[1] / abs(v[2]) + 1) * coef)]
    elif    v[0] < - abs(v[1])  and v[0] <= - abs(v[2]):
        idx3d = [1, math.floor((-v[1] / abs(v[0]) + 1) * coef) , math.floor((v[2] / abs(v[0]) + 1) * coef)]
    elif    v[1] <= - abs(v[2])  and v[1] < - abs(v[0]):
        idx3d = [2, math.floor((v[2] / abs(v[1]) + 1) * coef) , math.floor((v[0] / abs(v[1]) + 1) * coef)]
    elif    v[2] >= abs(v[0]) and v[2] > abs(v[1]):
        idx3d = [3, math.floor((v[0] / abs(v[2]) + 1) * coef) , math.floor((v[1] / abs(v[2]) + 1) * coef)]
    elif    v[0] >= abs(v[1])    and v[0] > abs(v[2]):
        idx3d = [4, math.floor((v[1] / abs(v[0]) + 1) * coef) , math.floor((-v[2] / abs(v[0]) + 1) * coef)]
    elif    v[1] > abs(v[2])    and v[1] >= abs(v[0]):
        idx3d = [5, math.floor((-v[2] / abs(v[1]) + 1) * coef) , math.floor((-v[0] / abs(v[1]) + 1) * coef)]
    else:
        idx3d = [0,0,0]
        
    return idx3d[0] * size * size + idx3d[1] * size + idx3d[2]
    
def Split(d_idx, cubemap_size):
    
    cube_hist = GenCubemap(d_idx.ravel(), cubemap_size)
    ShowHist(cube_hist)

    peak_idx = FindPeak(cube_hist)

    #if the mask is 1, it means this grid is in the peak.
    
    d_mask = np.zeros(d_idx.shape, order='F')
    SetMask(d_idx, d_mask, peak_idx)
    PlotMask(d_mask)
    
    dim = d_idx.shape
    
    imax = np.argmax(dim)
    print(imax)
    #oneslice = d_mask.sum(axis = ((imax + 1) % 3))
    #oneline = oneslice.sum(axis = ((imax + 1) % 3))
    histAxis = np.apply_over_axes(np.sum, d_mask, [(imax + 1) % 3,(imax + 2) % 3]).ravel()
    print(histAxis)
    plt.figure()
    plt.plot(histAxis)
    
    
    def gaus(x,a,x0,sigma):
        return a*exp(-(x-x0)**2/(2*sigma**2))
    
    n = histAxis.shape[0]
    x = np.array(range(n))
    y = histAxis
    mean = np.argmax(histAxis)
    sigma = n * 0.5 #np.sum(y*(x-mean)**2)/n
    peak = histAxis[mean]
    popt,pcov = curve_fit(gaus, x, y, p0=[peak,mean,sigma])
    plt.plot(x,gaus(x,*popt),'ro:',label='fit')
    plt.axvline(x=(popt[1] + 2 * popt[2]))
    print(popt)
    plt.show()
#    Split()

        
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

    
                