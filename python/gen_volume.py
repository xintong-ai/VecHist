# -*- coding: utf-8 -*-
"""
Created on Mon Jan  5 22:24:23 2015

@author: tong
"""
#import numpy as np
#import sample_two_halves
import sample_3d_ball
import write_vec
import numpy as np

d = sample_3d_ball.sample(2**6)
#d = np.array([[[[1,1,1], [2,2,2]], [[3,3,3], [4,4,4]]],[[[5,5,5], [6,6,6]], [[7,7,7], [8,8,8]]]])
write_vec.write(d, "D:/data/sample/test2.vec")

#d = sample_two_halves.sample(2**6)
#write_vec.write(d, "D:/data/sample/sample_two_halves.vec")