#!/usr/bin/env python

import numpy as np

a = np.array([[3,7],[9,1]])
print ('Org:')
print (a)
print ('sort:')
print (np.sort(a))
print ('sort as column:')
print (np.sort(a, axis =  0))
dt = np.dtype([('name',  'S10'),('age',  int)])
a = np.array([("raju",21),("anil",25),("ravi",  17),  ("amar",27)], dtype = dt)
print ('\norg:')
print (a)
print ('sort as name:')
print (np.sort(a, order =  'name'))


x = np.array([3,  1,  2])
print ('\norg:')
print (x)
print ('argsort with x:')
y = np.argsort(x)
print (y)
print ('argsort list org:')
print (x[y])


print ('\nsort_complex:')
print (np.sort_complex([5, 3, 6, 2, 1]))


print ('\nsort_partition:')
a = np.array([3, 4, 2, 1])
print (np.partition(a, 3))
print (np.partition(a, (1, 3)))


arr = np.array([46, 57, 23, 39, 1, 10, 0, 120])
print (arr[np.argpartition(arr, 2)[2]])


a = np.array([[30,40,0],[0,20,10],[50,0,60]])
print ('\norg:')
print (a)
print ('nonzero:')
print (np.nonzero (a))


x = np.arange(9.).reshape(3,  3)
print ('\norg:')
print (x)
print ('show array > 3 index:')
y = np.where(x >  3)
print (y)
print ('show array:')
print (x[y])


x = np.arange(9.).reshape(3,  3)
print ('\norg:')
print (x)
condition = np.mod(x,2)  ==  0
print ('condition:')
print (condition)
print ('extract:')
print (np.extract(condition, x))
