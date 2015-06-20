#%%
import numpy as np
import cubemap_coords as cube
import plot_vec as pv
import matplotlib.pyplot as plt
import time
import math

#Chris Jacobsen -
#This computes Super Quadrics for the Dark Sky data
#It is being commented out for now, until we get the Dark Sky data working or create a different flow for it
#cube.loadHaloComputeSuperquadric()
#pause

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

#Calculate entropy for root node (needed for tree visualization)
start = np.array([0, 0, 0])
dim = np.array([shape[0], shape[1], shape[2]])
m_idx = d_idx[start[0]:(start[0]+dim[0]), start[1]:(start[1]+dim[1]), start[2]:(start[2]+dim[2])]

cube_hist = cube.GenCubemap(m_idx.ravel(), cubemap_size)
entropy = cube.get_histogram_entropy(cube_hist.ravel())

######do the splitting computation########
root = cube.TreeNode(np.array([0, 0, 0]), np.array([shape[0], shape[1], shape[2]]), entropy, [0, 0, 0], np.array([[0,0,0], [0,0,0], [0,0,0]]), cube_hist)#np.array(d_idx.shape) / 2, np.array([0.0, 0.0, 0.0]))
cube.SplitEntropy(root, d_idx, d_3d, cubemap_size)

f = open('binary_tree.txt', 'w')
node_id = [0]
centers = []
vectors = []
entropys = []
eig_vals = []
eig_vecs = []
cube_hists = []
cube.writeBinaryTree(root, f, centers, vectors, entropys, eig_vals, eig_vecs, node_id, cube_hists, cubemap_size)
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
np.savetxt('cube_hists.txt', cube_hists, newline=" ")

time_end = time.time()
print(str(time_end - time_start) + " seconds")
