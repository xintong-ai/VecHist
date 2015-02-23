# -*- coding: utf-8 -*-
"""
Created on Tue Nov 11 17:38:33 2014

@author: tong
"""
def read(filename):
#filename = "D:/data/plume/15plume3d421.vec"
#    filename = "D:/data/isabel/UVWf01.vec"

    import struct
    import numpy as np
    
    f = open(filename, "rb")
    try:
        dim = [0 for i in range(3)]
        dim = struct.unpack('iii', f.read(12))
    #    dim[1] = struct.unpack('i', f.read(4))[0]
    #    dim[2] = struct.unpack('i', f.read(4))[0]
    #        print(dim)
        n = dim[0] * dim[1] * dim[2] * 3;
        d = np.zeros(n)
        d = np.fromfile(f, dtype=np.float32)
    #    print(dim[0] * dim[1] * dim[2] * 3)
    #    print((dim[0], dim[1], dim[2], 3))
    #    print(d.shape)
        d= np.reshape(d, (dim[0], dim[1], dim[2], 3))
    #        for x in range(dim[0]):
    #            for y in range(dim[1]):
    #                for z in range(dim[2]):
    #    #                tmp = 
    #    #                print(tmp)
    #                    d[x, y, z, :] = struct.unpack('fff', f.read(12))
    #    #    for i in range(100):
    #    #        print(d[0,0,i,:])
    finally:
        f.close()

    return d