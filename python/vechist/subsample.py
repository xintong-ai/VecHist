import read_vec
import write_vec

d = read_vec.read("D:/data/nek/nek.d_4.vec")
print(d.shape)
d_2 = d[::2,::2,::2]
print(d_2.shape)
write_vec.write(d_2, "D:/data/nek/nek.d_8.vec")