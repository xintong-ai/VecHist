# -*- coding: utf-8 -*-
"""
Created on Mon Jan  5 22:24:23 2015

@author: tong
"""
#import numpy as np
import sample_3d_ball
import write_vec

d = sample_3d_ball.sample(2**6)
write_vec.write(d, "D:/data/sample/test2.vec")