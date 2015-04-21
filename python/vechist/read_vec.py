# -*- coding: utf-8 -*-
"""
Created on Tue Nov 11 17:38:33 2014

@author: tong
"""
import re
#import MergeTreeNode

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

#Definition for a tree node with a list of children - used for the merge tree
class MergeTreeNode:
    def __init__(self):
        self.haloId = -1
        self.children = []

    def SetHaloId(self, newId):
        self.haloId = newId





#This routine will read the merger tree file at the specified location and load it into a basic data structure
def readMergerTree(filename):
    f = open(filename, "r")

    treeId = 0 #Default value for treeId
    dictionary = {} #Empty dictionary to hold references to the merger tree nodes
    root = None

    for line in f:
        #Ignore Comments
        if line[0] == '#':
            if len(line) >= 5 and line[0:5] == "#tree":
                data_int_list = [str(record) for record in line.split()]
                treeId = data_int_list[1]
                print("New tree record, id: " + treeId)
                dictionary.clear() #Wipe out the dictionary - we only use each dictionary once for each tree
                #if root != None:
                #    print("--------Printing current merge tree---------")
                #    printMergerTree(root, 0)
                #    print("--------Finished printing merge tree---------")
                root = None
            continue
        #Test the entry for the number of trees in the forest
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

            #Build a new tree node
            treeNode = MergeTreeNode()
            treeNode.SetHaloId(haloId)

            #Set the root node if needed
            if len(dictionary) == 0:
                root = treeNode

            #Store a reference to the treeNode in the dictionary
            dictionary[haloId] = treeNode

            #Look for the descendant and then link the new node to it in the tree
            if descendant_id in dictionary:
                parentNode = dictionary[descendant_id]
                parentNode.children.append(treeNode)
            elif descendant_id == -1:
                print("Descendant id was -1 - ignoring")
            else:
                print("WARNING!!!!!!!   Descendant id " + str(descendant_id) + " does not have a halo entry already in the dictionary!!!!!!")

#Uitility function to print the contents of the merge tree
#It currently only prints through 4 depth levels (starting from 0)
def printMergerTree(node, level):
    print("New Node from tree:")
    print("Halo id: " + str(node.haloId))
    print("Tree level: " + str(level))
    for childNode in node.children:
        if level <= 2:
            printMergerTree(childNode, level + 1)













