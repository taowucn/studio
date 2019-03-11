#!/usr/bin/env python

import numpy as np

a = np.arange(9, dtype = np.float_).reshape(3,3)
print ('first:')
print (a)
print ('sencond:')
b = np.array([10,10,10])
print (b)
print ('add:')
print (np.add(a,b))
print ('sub:')
print (np.subtract(a,b))
print ('multiply:')
print (np.multiply(a,b))
print ('divide:')
print (np.divide(a,b))


a = np.array([0.25,  1.33,  1,  100])
print ('org:')
print (a)
print ('reciprocal:')
print (np.reciprocal(a))


a = np.array([10,100,1000])
print ('org:')
print (a)
print ('power 2:')
print (np.power(a,2))
print ('second:')
b = np.array([1,2,3])
print (b)
print ('power array:')
print (np.power(a,b))


a = np.array([10,20,30])
b = np.array([3,5,7])
print ('First:')
print (a)

print ('Second:')
print (b)

print ('mod:')
print (np.mod(a,b))

print ('remainder():')
print (np.remainder(a,b))


