#!/usr/bin/env python

import numpy.matlib
import numpy as np

a = np.array([[1,2],[3,4]])
b = np.array([[11,12],[13,14]])
## dot: [[1*11+2*13, 1*12+2*14],[3*11+4*13, 3*12+4*14]]
print(np.dot(a,b))

# vdot: 1*11 + 2*12 + 3*13 + 4*14 = 130
print (np.vdot(a,b))

# inner: 1*0+2*1+3*0 = 2
print (np.inner(np.array([1,2,3]), np.array([0,1,0])))


a = np.array([[1,2], [3,4]])
print ('a:')
print (a)
b = np.array([[11, 12], [13, 14]])
print ('b:')
print (b)
print ('inner:')
print (np.inner(a,b))
## 1*11+2*12, 1*13+2*14
## 3*11+4*12, 3*13+4*14


a = [[1,0],[0,1]]
b = [[4,1],[2,2]]
print (np.matmul(a,b))


a = [[1,0],[0,1]]
b = [1,2]
print (np.matmul(a,b))
print (np.matmul(b,a))


a = np.arange(8).reshape(2,2,2)
b = np.arange(4).reshape(2,2)
print (np.matmul(a,b))


a = np.array([[1,2], [3,4]])
print (np.linalg.det(a))


b = np.array([[6,1,1], [4, -2, 5], [2,8,7]])
print (b)
print (np.linalg.det(b))
print (6*(-2*7 - 5*8) - 1*(4*7 - 5*2) + 1*(4*8 - -2*2))


x = np.array([[1,2],[3,4]])
y = np.linalg.inv(x)
print (x)
print (y)
print (np.dot(x,y))


a = np.array([[1,1,1],[0,2,5],[2,5,-1]])
print ('a:')
print (a)
ainv = np.linalg.inv(a)
print ('a linalg:')
print (ainv)
print ('b:')
b = np.array([[6],[-4],[27]])
print (b)
print ('a^(-1)b:')
x = np.linalg.solve(a,b)
print (x)
