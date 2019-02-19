#!/usr/bin/env python

import numpy as np

x = [1, 2, 3]
a = np.asarray(x)
print (a)

x =  [(1,2,3), (4,5)]
a = np.asarray(x)
print (a)

x =  [1,2,3]
a = np.asarray(x, dtype = float)
print (a)

s =  b'Hello World'
a = np.frombuffer(s, dtype =  'S1')
print (a)

# use range create list and iter
list=range(5)
it=iter(list)

# use iter create ndarray
x=np.fromiter(it, dtype=float)
print(x)

