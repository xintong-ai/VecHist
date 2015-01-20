# -*- coding: utf-8 -*-
"""
Created on Tue Nov 11 16:36:20 2014

@author: tong
"""

import numpy as np

def sample(n):
    

    #%% sample on a solid sphere
    
    d = np.zeros((n, n, n, 3), dtype=np.float32)
#    dv = np.zeros((n, n, n))   #valid or not
    
    
    for i in range(n):
        for j in range(n):
            for k in range(n):
                pos = [i, 0, 0]
                d[i, j, k, :] = pos
                
                
#print('filter from cube to sphere...')

#cnt = 0
#for i in range(n):
#    for j in range(n):
#        for k in range(n):
#            if dv[i,j,k] == 1 : 
#                d[cnt,:] = d_cube[i * n * n + j * n + k, :]
#                cnt = cnt + 1
#            else:
                
#d = d_cube
#d.resize((cnt,3))

    return d
                