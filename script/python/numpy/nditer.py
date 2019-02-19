#!/usr/bin/env python

import numpy as np

a = np.arange(6).reshape(2,3)
print ('In:')
print (a)
print ('Out:')
for x in np.nditer(a):
    print (x)

a = np.arange(6).reshape(2,3)
for x in np.nditer(a.T):
    print (x)
for x in np.nditer(a.T.copy(order='C')):
    print (x)


a = np.arange(0,60,5)
a = a.reshape(3,4)
print ('Original:')
print (a)
print ('\n')
for x in np.nditer(a, op_flags=['readwrite']):
    x[...]=2*x
print ('Modifyed:')
print (a)


a = np.arange(0,60,5)
a = a.reshape(3,4)
print  ('First:')
print (a)
print  ('\n')
print ('Second:')
b = np.array([1,  2,  3,  4], dtype =  int)
print (b)
print ('\n')
print ('Modifyed:')
for x,y in np.nditer([a,b]):
    print ("%d:%d"  %  (x,y))
