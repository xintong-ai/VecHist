# -*- coding: utf-8 -*-
"""
Created on Tue Nov 11 17:09:02 2014

@author: tong
"""

import numpy as np

def sample(n):
    #%% compute the random samples
    #http://www.bogotobogo.com/Algorithms/uniform_distribution_sphere.php
    #http://en.wikibooks.org/wiki/Mathematica/Uniform_Spherical_Distribution
    sample_rate = n/8;
    v = np.random.random_integers(0,sample_rate,n) /sample_rate
    phi = np.arccos(2 * v - 1)
    
    u = np.random.random_integers(0,sample_rate - 1,n) /sample_rate
    theta = 2 * np.pi * u
    
    d = np.zeros((n, 3))
    radius = 1
    d[:,0] = radius * np.cos(theta)*np.sin(phi);
    d[:,1] = radius * np.sin(theta)*np.sin(phi);
    d[:,2] = radius * np.cos(phi);
    
#    hist, bin_edges= np.histogram(theta, 10)
#    print(hist)
#    print(bin_edges)
    
    return d
                

