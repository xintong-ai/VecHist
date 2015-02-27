# -*- coding: utf-8 -*-
"""
Created on Sun Dec 14 23:42:54 2014

@author: tong
"""
import numpy as np
from numpy.linalg import norm
import math
import matplotlib.pyplot as plt
import copy
from scipy import asarray as ar,exp
from scipy.optimize import curve_fit
#from mayavi import mlab

#Definition for a  binary tree node
class TreeNode:
    def __init__(self):
#        self.val = x
        self.left = None
        self.right = None
        self.center = np.array([0, 0, 0])
        
    def SetCenter(p):
        self.center = p
    
    def insertRight(self):
        if self.right == None:
            self.right = TreeNode()
    
    def insertLeft(self):
        if self.left == None:
            self.left = TreeNode()

#class Solution:
    # @param root, a tree node
    # @return a list of integers
def inorderTraversal(root):
    stack = []
    node = root
    solution = []
    while node!= None or len(stack)>0:
        if node != None:
            stack.append(node)
            node = node.left
        else:
            node = stack.pop()
            solution.append(node.center)
            node = node.right
    return solution

def PlotMask(mask):
    dim = mask.shape
#    print(np.count_nonzero(mask))
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
#    peak_top = max_idx_1d
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
#    print(arr)

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
    
    return arr, max_idx_1d


def ShowHist(hist):
    size = hist.shape[1]
#    print(np.sum(hist))
        
#    max_val = size * size * 6 - 1
    max_val = np.max(hist)
    plt.figure()
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

def get3dCoords(idx, size):
    f = math.floor(idx / (size * size))
    a = math.floor(idx / size) - f * size
    b = idx % size
#    print("size:"+str(size))
    #coef = size * 0.5
    print("idx: "+str(idx))
    print("f: "+str(f))
    v = np.array([1,1,1])    
    a = a - size * 0.5
    b = b - size * 0.5    
    
    if(f == 0):
        v[0] = - v[2] * a
        v[1] = - v[2] * b
    elif(f == 1):
        v[1] = - v[0] * a
        v[2] =   v[0] * b
    elif(f == 2):
        v[2] =   v[1] * a
        v[0] =   v[1] * b
    elif(f == 3):
        v[0] =   v[2] * a
        v[1] =   v[2] * b
    elif(f == 4):
        v[1] =   v[0] * a
        v[2] = - v[0] * b
    elif(f == 5):
        v[2] = - v[1] * a
        v[0] = - v[1] * b

    return v / norm(v)
#            
#    if    v[2] < - abs(v[0])  and v[2] <= - abs(v[1]):
#        idx3d = [0, math.floor((-v[0] / abs(v[2]) + 1) * coef) , math.floor((-v[1] / abs(v[2]) + 1) * coef)]
#    elif    v[0] < - abs(v[1])  and v[0] <= - abs(v[2]):
#        idx3d = [1, math.floor((-v[1] / abs(v[0]) + 1) * coef) , math.floor((v[2] / abs(v[0]) + 1) * coef)]
#    elif    v[1] <= - abs(v[2])  and v[1] < - abs(v[0]):
#        idx3d = [2, math.floor((v[2] / abs(v[1]) + 1) * coef) , math.floor((v[0] / abs(v[1]) + 1) * coef)]
#    elif    v[2] >= abs(v[0]) and v[2] > abs(v[1]):
#        idx3d = [3, math.floor((v[0] / abs(v[2]) + 1) * coef) , math.floor((v[1] / abs(v[2]) + 1) * coef)]
#    elif    v[0] >= abs(v[1])    and v[0] > abs(v[2]):
#        idx3d = [4, math.floor((v[1] / abs(v[0]) + 1) * coef) , math.floor((-v[2] / abs(v[0]) + 1) * coef)]
#    elif    v[1] > abs(v[2])    and v[1] >= abs(v[0]):
#        idx3d = [5, math.floor((-v[2] / abs(v[1]) + 1) * coef) , math.floor((-v[0] / abs(v[1]) + 1) * coef)]
#    else:
#        idx3d = [0,0,0]
#        
#    return idx3d[0] * size * size + idx3d[1] * size + idx3d[2]


def get_histogram_dispersion(histogram):
    log2 = lambda x:math.log(x)/math.log(2)

    total = len(histogram)
    counts = {}
    for item in histogram:
        counts.setdefault(item,0)
        counts[item]+=1

    ent = 0
    for i in counts:
        if(counts[i]>0):
            p = float(counts[i])/total
            ent-=p*log2(p)
    return -ent*log2(1/ent)
    
def Split(centers, vectors, _d_idx, cubemap_size, _start_pos):
    d_idx = copy.deepcopy(_d_idx)
#    ret = TreeNode
    start_pos = copy.deepcopy(_start_pos)
    
    shape = d_idx.shape
    print(type(shape))
#    ret.center = start_pos + np.array(shape) * 0.5
    
    cube_hist = GenCubemap(d_idx.ravel(), cubemap_size)
    entropy = get_histogram_dispersion(cube_hist.ravel())
    print("entropy: " + str(entropy))
    print("shape: " + str(shape))
    #TODO: this threshold needs to change later

    ShowHist(cube_hist)

    peak_top = -1
    peak_idx, peak_top= FindPeak(cube_hist)

    if(entropy < 8 or shape[0] < 8 or shape[1] < 8 or shape[2] < 8):
        block_center  = start_pos + np.array(shape) * 0.5
        centers.append(block_center)
        vectors.append(get3dCoords(peak_top, cubemap_size))
        return
    #if the mask is 1, it means this grid is in the peak.
    
    d_mask = np.zeros(d_idx.shape, order='F')
    SetMask(d_idx, d_mask, peak_idx)
    PlotMask(d_mask)
    
    dim = d_idx.shape
    
    imax = np.argmax(dim)
#    print(imax)
    #oneslice = d_mask.sum(axis = ((imax + 1) % 3))
    #oneline = oneslice.sum(axis = ((imax + 1) % 3))
    histAxis = np.apply_over_axes(np.sum, d_mask, [(imax + 1) % 3,(imax + 2) % 3]).ravel()
#    print(histAxis)
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
    print("mid point:" + str(shape[imax] * 0.5))
    if(popt[1] > (shape[imax] * 0.5)):
        spl_pt = popt[1] - 1 * popt[2]
    else:
        spl_pt = popt[1] + 1 * popt[2]
    plt.plot(x,gaus(x,*popt),'ro:',label='fit')
    plt.axvline(x=spl_pt)
#    print(popt)
#    plt.show()

    side1 = []
    side2 = []    
    if(imax == 0):
        side1 = d_idx[:spl_pt, :, :]
        side2 = d_idx[spl_pt:, :, :]
    elif(imax == 1):
        side1 = d_idx[:, :spl_pt, :]
        side2 = d_idx[:, spl_pt:, :]
    else:
        side1 = d_idx[:, :, :spl_pt]
        side2 = d_idx[:, :, spl_pt:]
    
#    ret.insertLeft(ret)
#    ret.insertRight(ret)
    
    plt.show()
    
    Split(centers, vectors, side1, cubemap_size, start_pos)
    Split(centers, vectors, side2, cubemap_size, start_pos + side1.shape)
    
                