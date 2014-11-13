# -*- coding: utf-8 -*-
"""
Created on Tue Nov 11 17:04:48 2014

@author: tong
"""

import numpy as np

def sample(n):
    #%% compute uniform samples on sphere
#    The drawback is that there is always some patterns in the histogram
    #http://blog.marmakoide.org/?p=1
    
    golden_angle = np.pi * (3 - np.sqrt(5))
    theta = golden_angle * np.arange(n)
    z = np.linspace(1 - 1.0 / n, 1.0 / n - 1, n)
    radius = np.sqrt(1 - z * z) 
    
    d = np.zeros((n, 3))
    d[:,0] = radius * np.cos(theta)
    d[:,1] = radius * np.sin(theta)
    d[:,2] = z
    
    return d
                