#!/usr/bin/env python

import numpy as np

x = np.array([[1, 2], [3, 4], [5, 6]])
y = x[[0, 1, 2], [0, 1, 0]]
print (x)
print (y)

x = np.array([[  0,  1,  2],[  3,  4,  5],[  6,  7,  8],[  9,  10,  11]])
print ('\nOriginal Array:' )
print (x)
print ('\n')
rows = np.array([[0,0],[3,3]])
cols = np.array([[0,2],[0,2]])
y = x[rows,cols]
print  ('4 Corner Point:')
print (y)
print ('\n')

a = np.array([[1,2,3], [4,5,6],[7,8,9]])
b = a[1:3, 1:3]
c = a[1:3,[1,2]]
d = a[...,1:]
print (a)
print (b)
print (c)
print (d)
print (a[a > 5])

a = np.array([np.nan,1,2,np.nan,3,4,5])
print (a[~np.isnan(a)])
a = np.array([1, 2+6j, 5, 3.5+5j])
print (a[np.iscomplex(a)])

x=np.arange(32).reshape((8,4))
print (x)
print (x[[4,2,1,7]])
print (x[[-4,-2,-1,-7]])
print (x[np.ix_([1,5,7,2],[0,3,1,2])])

