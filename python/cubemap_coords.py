# -*- coding: utf-8 -*-
"""
Created on Sun Dec 14 23:42:54 2014

@author: tong
"""
import numpy as np

def get(v):
    
    if      v[0] < - abs(v[1])  and v[0] < - abs(v[2]):
        ret = [0, v[1] / v[0], v[2] / v[0]]
    elif    v[0] > abs(v[1])    and v[0] > abs(v[2]):
        ret = [1, v[1] / v[0], v[2] / v[0]]
    elif    v[1] < - abs(v[0])  and v[1] < - abs(v[2]):
        ret = [2, v[2] / v[1], v[0] / v[1]]
    elif    v[1] > abs(v[0])    and v[1] > abs(v[2]):
        ret = [3, v[2] / v[1], v[0] / v[1]]
    elif    v[2] < - abs(v[0])  and v[2] < - abs(v[1]):
        ret = [4, v[0] / v[2], v[1] / v[2]]
    else: #if    v[2] < - abs(v[0]) and v[2] < - abs(v[1]):
        ret = [5, v[0] / v[2], v[1] / v[2]]
        
        
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

    return ret
                