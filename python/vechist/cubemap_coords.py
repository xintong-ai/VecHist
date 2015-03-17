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
import time
#from mayavi import mlab

def sum_of_squares_of_digits(value):
    return sum(int(c) ** 2 for c in str(value))

def normalize(d):
    shape = d.shape
    d = np.reshape(d, (shape[0] * shape[1] * shape[2], 3))
    norm = np.linalg.norm(d, ord = 2, axis = 1)
    d = d / norm[:, np.newaxis]
    d = np.reshape(d, shape)
    return d

#Definition for a  binary tree node
class TreeNode:
    def __init__(self, start, dim, entropy, eig_val, eig_vec):
#        self.val = x
        self.left = None
        self.right = None
        #self.center = center
        #self.vector = vector
        #self.center = np.array([0.0, 0.0, 0.0])
        #self.vector = np.array([0.0, 0.0, 0.0])
        self.start = start
        self.dim = dim
        self.entropy = entropy
        self.eig_val = eig_val
        self.eig_vec = eig_vec
        
    def SetCenter(p):
        self.center = p
    
    #def insertRight(self):
    #    if self.right == None:
    #        self.right = TreeNode()
    
    #def insertLeft(self):
    #    if self.left == None:
    #        self.left = TreeNode()

#class Solution:
    # @param root, a tree node
    # @return a list of intenrs
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
    #plt.scatter(x, y, s = 2, color='white')
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
    
    nbin_half = int(math.floor(nbin / 2))
    for i in range(nbin_half):
        for j in range(nbin_half):
            a1 = abs(1 - i * delta)
            a2 = abs(1 - j * delta)
            sa = 4 * np.arcsin(np.sin(np.arctan(a1)) * np.sin(np.arctan(a2)))
            
            b1 = abs(1 - (i + 1) * delta)
            b2 = abs(1 - j * delta)
            sb = 4 * np.arcsin(np.sin(np.arctan(b1)) * np.sin(np.arctan(b2)))
    
            c1 = abs(1 - i * delta)
            c2 = abs(1 - (j + 1) * delta)
            sc = 4 * np.arcsin(np.sin(np.arctan(c1)) * np.sin(np.arctan(c2)))
    
            d1 = abs(1 - (i + 1) * delta)
            d2 = abs(1 - (j + 1) * delta)
            sd = 4 * np.arcsin(np.sin(np.arctan(d1)) * np.sin(np.arctan(d2)))
            
            solAng[i, j] = (sa - sb - sc + sd) * 0.25
            solAng[nbin - 1 - i, j] = solAng[i, j]
            solAng[i, nbin - 1 - j] = solAng[i, j]
            solAng[nbin - 1 - i, nbin - 1 - j] = solAng[i, j]
    
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
    #TODO: this is for the empty entries in DTI dataset
    hist[0] = 0
#    hist = np.arange(0, size * size * 6)       
#    print(hist.shape)
    hist = np.reshape(hist, (6 * size, size))
#    hist = np.ones((6 * size, size))
    
    #normalize by solid angles
    scale_factor = 1000
    sol_ang = SolidAngles(size)
    #plt.imshow(sol_ang)
    #plt.colorbar()
    #plt.show()
    for i in range(0, 6):
        hist[i*size:(i+1)*size, :] = hist[i*size:(i+1)*size, :] / (sol_ang * scale_factor)
    #print('Solid Angle (size' + str(size) + '):')
    #print(sol_ang)
    
    #normalize to be a histogram
    hist_sum = np.sum(hist)
    #print(hist_sum)
    if(hist_sum < 0):
        print("error: bin sum is negative...")
    hist = hist / (hist_sum * 1.0)
#    ShowHist(hist)
    
    return hist

def GenCubemapCut(d, size):
    [hist, bin_edges] = np.histogram(d, np.arange(0, size * size * 6 + 1))
#    hist = np.arange(0, size * size * 6)
#    print(hist.shape)
    hist = np.reshape(hist, (6 * size, size))
#    hist = np.ones((6 * size, size))

    #normalize by solid angles
    #scale_factor = 1000
    #sol_ang = SolidAngles(size)
    ###for i in range(0, 6):
    ###    hist[i*size:(i+1)*size, :] = hist[i*size:(i+1)*size, :] / (sol_ang * scale_factor)
    #print('Solid Angle (size' + str(size) + '):')
    #print(sol_ang)

    #normalize to be a histogram
    #hist_sum = np.sum(hist)
    #print(hist_sum)
    #if(hist_sum < 0):
    #    print("error: bin sum is negative...")
    ###hist = hist / (hist_sum * 1.0)
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
    
    #when f = 0, 1, 2, the dominant direction is negative, otherwise they are positive
    if(f <3):
        v = np.array([-1.0,-1.0,-1.0])    
    else:
        v = np.array([1.0,1.0,1.0])    

    a = float(a) / (size * 0.5) - 1
    b = float(b) / (size * 0.5) - 1    
    
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


def get_histogram_entropy(histogram):
    log2 = lambda x:math.log(x)/math.log(2)
    ent = 0
    for p in histogram:
        if(p>0):
            ent-=p*log2(p)
    return ent
    
def Split(ret, _d_idx, cubemap_size, _start_pos, tree_depth):
    d_idx = copy.deepcopy(_d_idx)
    #ret = TreeNode
    start_pos = copy.deepcopy(_start_pos)
    
    shape = d_idx.shape
    print(type(shape))
    
    cube_hist = GenCubemap(d_idx.ravel(), cubemap_size)
    entropy = get_histogram_dispersion(cube_hist.ravel())
    print("entropy: " + str(entropy))
    print("shape: " + str(shape))
    #TODO: this threshold needs to change later

    #ShowHist(cube_hist)

    peak_top = -1
    peak_idx, peak_top= FindPeak(cube_hist)

    #if(entropy < 8 or shape[0] < 8 or shape[1] < 8 or shape[2] < 8):
    if(tree_depth == 0):
        block_center  = start_pos + np.array(shape) * 0.5
        ret.center = block_center
        #centers.append(block_center)
        ret.vector = get3dCoords(peak_top, cubemap_size)
        #vectors.append(get3dCoords(peak_top, cubemap_size))
        return
    #if the mask is 1, it means this grid is in the peak.
    
    d_mask = np.zeros(d_idx.shape, order='F')
    SetMask(d_idx, d_mask, peak_idx)
    #PlotMask(d_mask)
    
    dim = d_idx.shape
    
    imax = np.argmax(dim)
#    print(imax)
    #oneslice = d_mask.sum(axis = ((imax + 1) % 3))
    #oneline = oneslice.sum(axis = ((imax + 1) % 3))
    histAxis = np.apply_over_axes(np.sum, d_mask, [(imax + 1) % 3,(imax + 2) % 3]).ravel()
#    print(histAxis)
    #plt.figure()
    #plt.plot(histAxis)
    
    
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
    #plt.plot(x,gaus(x,*popt),'ro:',label='fit')
    #plt.axvline(x=spl_pt)
#    print(popt)
#    plt.show()

    side1 = []
    side2 = []
    start_pos_2 = copy.deepcopy(start_pos)    
    if(imax == 0):
        side1 = d_idx[:spl_pt, :, :]
        side2 = d_idx[spl_pt:, :, :]
        start_pos_2[0] += spl_pt
    elif(imax == 1):
        side1 = d_idx[:, :spl_pt, :]
        side2 = d_idx[:, spl_pt:, :]
        start_pos_2[1] += spl_pt
    else:
        side1 = d_idx[:, :, :spl_pt]
        side2 = d_idx[:, :, spl_pt:]
        start_pos_2[2] += spl_pt
    
    #ret.insertLeft()
    #ret.insertRight()
    ret.left = TreeNode(start_pos, side1.shape)
    ret.right = TreeNode(start_pos_2, side2.shape)
    
    #plt.show()
    
    Split(ret.left, side1, cubemap_size, start_pos, tree_depth - 1)
    Split(ret.right, side2, cubemap_size, start_pos_2, tree_depth - 1)

def PCA(d):
    d2 = np.zeros((d.shape[0] * 2, 3));
    d2[: d.shape[0], :] = copy.deepcopy(d)
    d2[d.shape[0]:, :] = copy.deepcopy(d*(-1))
    cov_mat = np.cov([d2[:, 0], d2[:, 1], d2[:, 2]])
    eig_val, eig_vec = np.linalg.eig(cov_mat)
    eig_pairs = [(np.abs(eig_val[i]), eig_vec[:, i]) for i in range(len(eig_val))]
    eig_pairs.sort()
    eig_pairs.reverse()
    return [eig_pairs[0][0], eig_pairs[1][0], eig_pairs[2][0]], np.array([eig_pairs[0][1], eig_pairs[1][1], eig_pairs[2][1]])

#d is the original data array with dimension [z, y, x, v] with C order
def SplitEntropy(ret, _d_idx, d_3d, cubemap_size):
    #d_idx = copy.deepcopy(_d_idx)
    #start_pos = copy.deepcopy(ret.start)
 #   shape = ret.dim#d_idx.shape
    imax = np.argmax(ret.dim)

    side1 = []
    side2 = []

    entropy_sum = []
    m_idx = _d_idx[ret.start[0]:(ret.start[0]+ret.dim[0]), 
                   ret.start[1]:(ret.start[1]+ret.dim[1]), 
                   ret.start[2]:(ret.start[2]+ret.dim[2])]

    m_3d = d_3d[ret.start[2]:(ret.start[2]+ret.dim[2]), 
                ret.start[1]:(ret.start[1]+ret.dim[1]), 
                ret.start[0]:(ret.start[0]+ret.dim[0]), 
                :]

    #print('Final Entropy Sum:')
    #print(entropy_sum)

    startT = time.time()
    if(imax == 0):
        side1 = m_idx[:1, :, :]
        side2 = m_idx[1:, :, :]
    elif(imax == 1):
        side1 = m_idx[:, :1, :]
        side2 = m_idx[:, 1:, :]
    else:
        side1 = m_idx[:, :, :1]
        side2 = m_idx[:, :, 1:]
    print("percentage: " + str(float(1) / ret.dim[imax]))
    cube_hist_1 = GenCubemapCut(side1.ravel(), cubemap_size)
    cube_hist_2 = GenCubemapCut(side2.ravel(), cubemap_size)

    scale_factor = 1000
    sol_ang = SolidAngles(cubemap_size)
    norm_hist1 = copy.deepcopy(cube_hist_1)
    for i in range(0, 6):
        norm_hist1[i*cubemap_size:(i+1)*cubemap_size, :] = cube_hist_1[i*cubemap_size:(i+1)*cubemap_size, :] / (sol_ang * scale_factor)
    hist_sum = np.sum(norm_hist1)
    if(hist_sum < 0):
        print("error: bin sum is negative...")
    norm_hist1 = norm_hist1 / (hist_sum * 1.0)

    scale_factor = 1000
    sol_ang = SolidAngles(cubemap_size)
    norm_hist2 = copy.deepcopy(cube_hist_2)
    for i in range(0, 6):
        norm_hist2[i*cubemap_size:(i+1)*cubemap_size, :] = cube_hist_2[i*cubemap_size:(i+1)*cubemap_size, :] / (sol_ang * scale_factor)
    hist_sum = np.sum(norm_hist2)
    if(hist_sum < 0):
        print("error: bin sum is negative...")
    norm_hist2 = norm_hist2 / (hist_sum * 1.0)

    #print("Cube hist 1:")
    #print(norm_hist1)
    #print("Cube hist 2:")
    #print(norm_hist2)
    entropy_1 = get_histogram_entropy(norm_hist1.ravel())
    #print "Entropy 1: " + str(entropy_1)
    entropy_2 = get_histogram_entropy(norm_hist2.ravel())
    #print "Entropy 2: " + str(entropy_2)
    p1 = float(1) / ret.dim[imax]
    entropy_sum.append(entropy_1 * p1 + entropy_2 * ( 1.0 - p1))
    #print('p1: ' + str(p1))
    #print('Current entropy sum:')
    #print(entropy_sum)

    for spl_pt in range(2, ret.dim[imax]):
        if(imax == 0):
            side = m_idx[spl_pt - 1, :, :]
        elif(imax == 1):
            side = m_idx[:, spl_pt - 1, :]
        else:
            side = m_idx[:, :, spl_pt - 1]
        print("percentage: " + str(float(spl_pt) / ret.dim[imax]))
        cube_hist_single = GenCubemapCut(side.ravel(), cubemap_size)
        cube_hist_1 = cube_hist_1 + cube_hist_single
        cube_hist_2 = cube_hist_2 - cube_hist_single

        scale_factor = 1000
        sol_ang = SolidAngles(cubemap_size)
        norm_hist1 = copy.deepcopy(cube_hist_1)
        for i in range(0, 6):
            norm_hist1[i*cubemap_size:(i+1)*cubemap_size, :] = cube_hist_1[i*cubemap_size:(i+1)*cubemap_size, :] / (sol_ang * scale_factor)
        hist_sum = np.sum(norm_hist1)
        if(hist_sum < 0):
            print("error: bin sum is negative...")
        norm_hist1 = norm_hist1 / (hist_sum * 1.0)

        scale_factor = 1000
        sol_ang = SolidAngles(cubemap_size)
        norm_hist2 = copy.deepcopy(cube_hist_2)
        for i in range(0, 6):
            norm_hist2[i*cubemap_size:(i+1)*cubemap_size, :] = cube_hist_2[i*cubemap_size:(i+1)*cubemap_size, :] / (sol_ang * scale_factor)
        hist_sum = np.sum(norm_hist2)
        if(hist_sum < 0):
            print("error: bin sum is negative...")
        norm_hist2 = norm_hist2 / (hist_sum * 1.0)

        #print("Cube hist 1:")
        #print(norm_hist1)
        #print("Cube hist 2:")
        #print(norm_hist2)
        entropy_1 = get_histogram_entropy(norm_hist1.ravel())
        #print "Entropy 1: " + str(entropy_1)
        entropy_2 = get_histogram_entropy(norm_hist2.ravel())
        #print "Entropy 2: " + str(entropy_2)
        p1 = float(spl_pt) / ret.dim[imax]
        entropy_sum.append(entropy_1 * p1 + entropy_2 * ( 1.0 - p1))
        max_val = np.max(cube_hist_1) * 0.2
        #print('p1: ' + str(p1))
        #print('Current entropy sum:')
        #print(entropy_sum)

    #print('Final Entropy Sum:')
    #print(entropy_sum)
    endT = time.time()
    print('Total time for optimized split is: ' + str(endT - startT))

    # print('--------------------------------------------------------------')
    # print('End of Optimized Version')
    # print('--------------------------------------------------------------')



    spl_pt = np.argmin(np.array(entropy_sum)) + 1

    #plt.plot(entropy_sum)
    #plt.show()
    start_pos_2 = copy.deepcopy(ret.start)    
    if(imax == 0):
        side1 = m_idx[:spl_pt, :, :]
        side2 = m_idx[spl_pt:, :, :]
        side1_3d = m_3d[:, :, :spl_pt, :]
        side2_3d = m_3d[:, :, spl_pt:, :]
        start_pos_2[0] += spl_pt
    elif(imax == 1):
        side1 = m_idx[:, :spl_pt, :]
        side2 = m_idx[:, spl_pt:, :]
        side1_3d = m_3d[:, :spl_pt, :, :]
        side2_3d = m_3d[:, spl_pt:, :, :]
        start_pos_2[1] += spl_pt
    else:
        side1 = m_idx[:, :, :spl_pt]
        side2 = m_idx[:, :, spl_pt:]
        side1_3d = m_3d[:spl_pt, :, :, :]
        side2_3d = m_3d[spl_pt:, :, :, :]
        start_pos_2[2] += spl_pt

    side1_3d = np.reshape(side1_3d, (-1,3))
    side2_3d = np.reshape(side2_3d, (-1,3))
    
    eig_val_1, eig_vec_1 = PCA(side1_3d)
    eig_val_2, eig_vec_2 = PCA(side2_3d)

    cube_hist_1 = GenCubemap(side1.ravel(), cubemap_size)
    cube_hist_2 = GenCubemap(side2.ravel(), cubemap_size)


    entropy_1 = get_histogram_entropy(cube_hist_1.ravel())
    entropy_2 = get_histogram_entropy(cube_hist_2.ravel())

    threshold = 8
    print("entropy_1: " + str(entropy_1))
    print("entropy_2: " + str(entropy_2))
    ret.left = TreeNode(ret.start, side1.shape, entropy_1, eig_val_1, eig_vec_1)
    ret.right = TreeNode(start_pos_2, side2.shape, entropy_2, eig_val_2, eig_vec_2)
    if ret.left.entropy > threshold:# or cube_hist_1[0, 0] > 0.999:
        SplitEntropy(ret.left, _d_idx, d_3d, cubemap_size)
    if ret.right.entropy > threshold:# or cube_hist_2[0, 0] > 0.999:
        SplitEntropy(ret.right, _d_idx, d_3d, cubemap_size)
    
    
def writeBinaryTree(node, f, starts, dims, entropys, eig_vals, eig_vecs, node_id):
    if node == None:
        f.write("-1 ")
    else:
        #f.write(str(node.center[0]) + " " + str(node.center[1]) + " " + str(node.center[2]) + " ")
        #f.write(str(node.vector[0]) + " " + str(node.vector[1]) + " " + str(node.vector[2]) + " ")
        f.write(str(node_id[0]) + " ")
        starts.append(node.start)
        dims.append(node.dim)
        entropys.append(node.entropy)
        eig_vals.append(node.eig_val)
        eig_vecs.append(node.eig_vec[0, :])
        eig_vecs.append(node.eig_vec[1, :])
        eig_vecs.append(node.eig_vec[2, :])
        node_id[0] += 1
        #out << p->data << " ";
        writeBinaryTree(node.left, f, starts, dims, entropys, eig_vals, eig_vecs, node_id)
        writeBinaryTree(node.right, f, starts, dims, entropys, eig_vals, eig_vecs, node_id)
