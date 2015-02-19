# -*- coding: utf-8 -*-
"""
Created on Mon Jan  5 22:45:26 2015

@author: tong
"""

# -*- coding: utf-8 -*-
"""
Created on Tue Nov 11 16:36:20 2014

@author: tong
"""

import numpy as np

def sample(n):
    

    #%% sample on a solid sphere
    
    
    d = np.zeros((n, n * 2, n * 3, 3), dtype=np.float32)
#    dv = np.zeros((n, n, n))   #valid or not
    center = [d.shape[0] * 0.5, d.shape[1] * 0.5, d.shape[2] * 0.5]
    
    
    for i in range(d.shape[0]):
        for j in range(d.shape[1]):
            for k in range(d.shape[2]):
                pos = [i, j, k]
                vec = np.subtract(pos, center)
                dis = np.linalg.norm(vec)
                val = 0
                if ( dis > 0.01):# and (dis > (n - 1) * 0.25):
                    val = vec / dis
                else:
                    val = [1,0,0]
                d[i, j, k, :] = val

    return d
    