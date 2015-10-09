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
        n = dim[0] * dim[1] * dim[2]
        
        print(n)
#        print(d)
        d_new = np.zeros((n, 3), dtype=np.float32)
        for k in range(0, dim[2]):
            for j in range(0, dim[1]):
                for i in range(0, dim[0]):
                    d_new[k * dim[0] * dim[1] + j * dim[0] + i,:] = d[i, j, k, :]
#        print(d[0,0,0,:])
#        print(d[0,1,0,:])
#        print(d_new)     
        d_new = np.reshape(d_new, n * 3)
        print(type(d_new[0]))
        d_new.tofile(f)
    finally:
        f.close()
