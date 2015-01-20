# -*- coding: utf-8 -*-
"""
Created on Mon Jan  5 22:32:43 2015

@author: tong
"""

# -*- coding: utf-8 -*-
"""
Created on Tue Nov 11 17:38:33 2014

@author: tong
"""
def write(d, filename):
#filename = "D:/data/plume/15plume3d421.vec"
#    filename = "D:/data/isabel/UVWf01.vec"

#    import struct
    import numpy as np
    import struct
    
    f = open(filename, "wb")
    try:
        dim = d.shape
        print(type(dim[0]))
        print(struct.pack('3i', dim[0], dim[1], dim[2]))
#        f.write(dim[0])
#        f.write(dim[1])
#        f.write(dim[2])
        f.write(struct.pack('3i', dim[0], dim[1], dim[2]))
#        np.save(f, dim)
        print(dim)
        n = dim[0] * dim[1] * dim[2] * 3
        print(n)
        d= np.reshape(d, n)
        d.tofile(f)
    finally:
        f.close()
