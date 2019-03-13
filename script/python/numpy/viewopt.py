#!/usr/bin/env python

import numpy as np

a = np.arange(6)
print ('Org:')
print (a)
print ('a id:')
print (id(a))
print ('b=a:')
b = a
print (b)
print ('b id:')
print (id(b))
print ('chang b shape:')
b.shape = 3,2
print (b)
print ('a shape:')
print (a)


a = np.arange(6).reshape(3,2)
print ('\na:')
print (a)
print ('creat a view:')
b = a.view()
print (b)
print ('two array has different id')
print ('a id:')
print (id(a))
print ('b id:' )
print (id(b))
b.shape =  2,3
print ('b shape:')
print (b)
print ('a shape:')
print (a)


arr = np.arange(12)
print ('\nOrg:')
print (arr)
print ('create cut:')
a=arr[3:]
b=arr[3:]
a[1]=123
b[2]=234
print(arr)
print(id(a),id(b),id(arr[3:]))


a = np.array([[10,10],  [2,3],  [4,5]])
print ('\na:')
print (a)
print ('a copy:')
b = a.copy()
print ('b:')
print (b)
print ('is b same as a:')
print (b is a)
print ('modified b')
b[0,0]  =  100
print ('modifed b:')
print (b)
print ('a nothing chang:')
print (a)
