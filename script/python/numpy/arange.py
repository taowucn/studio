#!/usr/bin/env python

import numpy as np

x = np.arange(5)
print (x)

x = np.arange(5, dtype =  float)
print (x)

x = np.arange(10, 20, 2)
print (x)

a = np.linspace(1,10,10)
print (a)

a = np.linspace(1,1,10)
print (a)

a = np.linspace(10, 20, 5, endpoint = False)
print (a)

a = np.linspace(1, 10, 10, retstep=True)
print (a)

b = np.linspace(1, 10, 10).reshape([10, 1])
print (b)

a = np.logspace(1.0, 2.0, num=2)
print (a)

a = np.logspace(0, 9, 10, base=2)
print (a)
