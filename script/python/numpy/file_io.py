#!/usr/bin/env python

import numpy as np

a = np.array([1,2,3,4,5])
np.save('outfile.npy',a)
np.save('outfile2',a)

b = np.load('outfile.npy')
print (b)


a = np.array([[1,2,3],[4,5,6]])
b = np.arange(0, 1.0, 0.1)
c = np.sin(b)
np.savez("outfile3.npz", a, b, sin_array = c)
r = np.load("outfile3.npz")
print(r.files)
print(r["arr_0"])
print(r["arr_1"])
print(r["sin_array"])


a = np.array([1,2,3,4,5])
np.savetxt('out.txt',a)
b = np.loadtxt('out.txt')
print(b)


a=np.arange(0,10,0.5).reshape(4,-1)
np.savetxt("out1.txt",a,fmt="%d",delimiter=",")
b = np.loadtxt("out1.txt",delimiter=",")
print(b)
