#!/usr/bin/env python

import numpy as np

a = np.array([[3,7,5],[8,4,3],[2,4,9]])
print ('org:')
print (a)
print ('amin 1:')
print (np.amin(a,1))
print ('amin 0:')
print (np.amin(a,0))
print ('amax:')
print (np.amax(a))
print ('amax axis 0:')
print (np.amax(a, axis = 0))


a = np.array([[3,7,5],[8,4,3],[2,4,9]])
print ('Org:')
print (a)
print ('ptp:')
print (np.ptp(a))
print ('ptp 1:')
print (np.ptp(a, axis = 1))
print ('ptp 0:')
print (np.ptp(a, axis = 0))


a = np.array([[10, 7, 4], [3, 2, 1]])
print ('\nOrg:')
print (a)
print ('percentile:')
print (np.percentile(a, 50))
print (np.percentile(a, 50, axis=0))
print (np.percentile(a, 50, axis=1))
print (np.percentile(a, 50, axis=1, keepdims=True))


a = np.array([[30,65,70],[80,95,10],[50,90,60]])
print ('\nOrg:')
print (a)
print (np.median(a))
print (np.median(a, axis =  0))
print (np.median(a, axis =  1))


a = np.array([[1,2,3],[3,4,5],[4,5,6]])
print ('\nOrg:')
print (a)
print (np.mean(a))
print (np.mean(a, axis =  0))
print (np.mean(a, axis =  1))


a = np.array([1,2,3,4])
print ('\nOrg:')
print (a)
print ('average:')
print (np.average(a))
wts = np.array([4,3,2,1])
print ('average again:')
print (np.average(a,weights = wts))
print ('sum weight:')
print (np.average([1,2,3,  4],weights =  [4,3,2,1], returned =  True))


a = np.arange(6).reshape(3,2)
print ('\nOrg:')
print (a)
print ('average weight:')
wt = np.array([3,5])
print (np.average(a, axis =  1, weights = wt))
print ('average weight:')
print (np.average(a, axis =  1, weights = wt, returned =  True))


print ('\nstd:')
print (np.std([1,2,3,4]))

print ('\nvar:')
print (np.var([1,2,3,4]))

