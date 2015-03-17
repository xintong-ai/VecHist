#%%
import numpy as np
import read_vec
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

d_idx = np.load('d_idx.npy')
#plt.imshow(d_idx[math.floor(d_idx.shape[0]/2), :, :])
#plt.show()
#plt.imshow(d_idx[:, math.floor(d_idx.shape[1]/2), :])
#plt.show()
#plt.imshow(d_idx[:, :, math.floor(d_idx.shape[2]/2)])
#plt.show()
shape = d_idx.shape

#d_3d = read_vec.read("D:/data/isabel/UVWf01.vec")
#d_3d = read_vec.read("/home/datahead/research/rawData/15plume3d430.vec")
d_3d = read_vec.read("D:/data/plume/15plume3d421.vec")

d_3d = cube.normalize(d_3d)
#d_3d = read_vec.read("D:/data/brain_dti/vector-field.vec")
#d_3d = read_vec.read("D:/data/nek/nek.d_4.vec")

print(d_3d.shape)

root = cube.TreeNode(np.array([0, 0, 0]), np.array([shape[0], shape[1], shape[2]]), 0, [0, 0, 0], np.array([[0,0,0], [0,0,0], [0,0,0]]))#np.array(d_idx.shape) / 2, np.array([0.0, 0.0, 0.0]))
#centers = []
#vectors = []
#tree_depth = 3;
cube.SplitEntropy(root, d_idx, d_3d, cubemap_size)

f = open('binary_tree.txt', 'w')
node_id = [0]
centers = []
vectors = []
entropys = []
eig_vals = []
eig_vecs = []
cube.writeBinaryTree(root, f, centers, vectors, entropys, eig_vals, eig_vecs, node_id)
eig_vals = np.array(eig_vals)
eig_vecs = np.array(eig_vecs)
f.write("\n")
centers = np.array(centers)
vectors = np.array(vectors)
np.savetxt('starts.txt', centers, newline=" ")
np.savetxt('dims.txt', vectors, newline=" ")
np.savetxt('entropys.txt', entropys, newline=" ")
np.savetxt('eig_vals.txt', eig_vals, newline=" ")
np.savetxt('eig_vecs.txt', eig_vecs, newline=" ")

time_end = time.time()
print(str(time_end - time_start) + " seconds")


##allcenters = cube.inorderTraversal(root)
#centers_array = np.array(centers)
#vectors_array = np.array(vectors)

##import numpy as np
#from mpl_toolkits.mplot3d import Axes3D
##import matplotlib.pyplot as plt

#fig = plt.figure()    
#ax = fig.gca(projection='3d')
#ax.set_aspect('equal')
##print(vectors[:, 2])
#print(centers_array.shape)
#print(type(centers_array[0]))
#ax.quiver(centers_array[:, 0], centers_array[:, 1], centers_array[:, 2], vectors_array[:, 0], vectors_array[:, 1], vectors_array[:, 2], linewidths=10)
#ax.set_xlim3d(0, d_idx.shape[0])
#ax.set_ylim3d(0, d_idx.shape[1])
#ax.set_zlim3d(0, d_idx.shape[2])
#plt.show()

