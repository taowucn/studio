#!/usr/bin/env python

import numpy as np

print ('\n## reshape ##')
a = np.arange(8)
print ('Original: ')
print (a)
print ('\n')
b = a.reshape(4,2)
print ('Modifyed:')
print (b)


print ('\n## flat ##')
a = np.arange(9).reshape(3,3)
print ('Original:')
for row in a:
    print (row)
print ('Iterater:')
for element in a.flat:
    print (element)


print ('\n## flatten ##')
a = np.arange(8).reshape(2,4)
print ('Original:')
print (a)
print ('Flatten:')
print (a.flatten())
print ('Print with Flattern:')
print (a.flatten(order = 'F'))


print ('\n## rvael ##')
a = np.arange(8).reshape(2,4)
print ('Org:')
print (a)
print ('ravel:')
print (a.ravel())
print ('ravel with F')
print (a.ravel(order = 'F'))


print ('\n## transpose ##')
a = np.arange(12).reshape(3,4)
print ('Org:')
print (a )
print ('transpose:')
print (np.transpose(a))
print ('T:')
print (a.T)
print ('transpose(1,0):')
print (a.transpose(1,0))

print ('\n## rollaxis ##')
# create 3 dim ndarray
a = np.arange(8).reshape(2,2,2)
print ('Org:')
print (a)
print ('\n')
print ('rollaxis (2 <-> 0):')
print (np.rollaxis(a,2))
print ('\n')
print ('rollaxis (0 <-> 1):')
print (np.rollaxis(a,2,1))


print ('\n## swapaxes ##')
a = np.arange(8).reshape(2,2,2)
print ('Org:')
print (a)
print ('\n')
print ('swapaxes (0 <-> 2):')
print (np.swapaxes(a, 2, 0))


a = np.arange(12).reshape(2,2,3)
print (a.shape[0])


