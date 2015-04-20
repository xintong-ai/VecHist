#%%
import numpy as np
import cubemap_coords as cube
import plot_vec as pv
import matplotlib.pyplot as plt
import time
import math

time_start = time.time()

#d = sample_ball.sample(2**8)
#d = sample_uniform.sample(2**20)
#d = sample_random.sample(2**24)
#d = read_vec.read("D:/data/isabel/UVWf01.vec")
#d = read_vec.read("D:/data/plume/15plume3d421.vec")
#d = read_vec.read("D:/data/sample/test3.vec")

cubemap_size = 16

#######Load data#########
d_idx = np.load('d_idx.npy')
shape = d_idx.shape

d_3d = np.load('d_vec.npy')
d_3d = cube.normalize(d_3d)

######do the splitting computation########
root = cube.TreeNode(np.array([0, 0, 0]), np.array([shape[0], shape[1], shape[2]]), 0, [0, 0, 0], np.array([[0,0,0], [0,0,0], [0,0,0]]))#np.array(d_idx.shape) / 2, np.array([0.0, 0.0, 0.0]))
cube.SplitEntropy(root, d_idx, d_3d, cubemap_size)

f = open('binary_tree.txt', 'w')
node_id = [0]
centers = []
vectors = []
entropys = []
eig_vals = []
eig_vecs = []
cube.writeBinaryTree(root, f, centers, vectors, entropys, eig_vals, eig_vecs, node_id)
f.write("\n")
f.close()

#########Save the attributes of the tree nodes##########
eig_vals = np.array(eig_vals)
eig_vecs = np.array(eig_vecs)
centers = np.array(centers)
vectors = np.array(vectors)
np.savetxt('starts.txt', centers, newline=" ")
np.savetxt('dims.txt', vectors, newline=" ")
np.savetxt('entropys.txt', entropys, newline=" ")
np.savetxt('eig_vals.txt', eig_vals, newline=" ")
np.savetxt('eig_vecs.txt', eig_vecs, newline=" ")

time_end = time.time()
print(str(time_end - time_start) + " seconds")
