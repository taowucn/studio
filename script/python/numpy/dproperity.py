#!/usr/bin/env python

import numpy as np

a = np.arange(24)
print (a.ndim) # one dimen
b = a.reshape(2, 4, 3) # three dimen
print (b.ndim)

a = np.array([[1, 2, 3], [4, 5, 6]])
b = a.reshape(3, 2)
print (a.shape)
a.shape = (3, 2)
print (a)
print (b)

x = np.array([1,2,3,4,5], dtype = np.int8)
print (x.itemsize)
y = np.array([1,2,3,4,5], dtype = np.float64)
print (y.itemsize)

print (x.flags)
