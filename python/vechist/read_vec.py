# -*- coding: utf-8 -*-
"""
Created on Tue Nov 11 17:38:33 2014

@author: tong
"""
import re

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
        d= np.reshape(d, (dim[2], dim[1], dim[0], 3))
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

#This routine will read the merger tree file at the specified location and load it into a basic data structure
def readMergerTree(filename):
    f = open(filename, "r")

    treeId = 0 #Default value

    for line in f:
        #Ignore Comments
        if line[0] == '#':
            if len(line) >= 5 and line[0:5] == "#tree":
                data_int_list = [str(record) for record in line.split()]
                treeId = data_int_list[1]
                print("New tree record, id: " + treeId)
            continue
        #Test for the entry for the number of trees in the forest
        match = re.match(r"^[0-9]+ *$", line)
        if match:
            numTrees = int(line)
            print("Found the (apparent) number of tree elements: " + str(numTrees) )
        else:
            data_int_list = [str(record) for record in line.split()]
            #print(data_int_list)
            print("New Halo Record:")
            timestep = float(data_int_list[0])
            print("Timestep: " + str(timestep))
            haloId = int(float(data_int_list[1]))
            print("Halo Id: " + str(haloId))
            num_progenitors = int(float(data_int_list[4]))
            print("Number of Progenitors: " + str(num_progenitors))
            descendant_id = int(float(data_int_list[3]))
            print("Descendant id: " + str(descendant_id))
            print("\n")




