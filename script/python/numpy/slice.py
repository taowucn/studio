#!/usr/bin/env python

import numpy as np

a = np.arange(10)
b = a[2:7:2]
print (a)
print (b)
print (a[2:])
print (a[:-1])

a = np.array([[1,2,3],[4,5,6],[7,8,9]])
print (a)
print (a[1:])

print (a[...,1])   # 2th column
print (a[1,...])   # 2th row
print (a[...,1:])  # all elements after 2th column

a = np.array([[1,2,3],[4,5,6],[7,8,9]])
print ("Test:")
print (a[:,1])
