#!/usr/bin/env python

import numpy as np

a = np.array([1,  256,  8755], dtype = np.int16)
print ('Org:')
print (a)
print ('show memory content with hex:')
print (map(hex,a))
print ('byteswap:')
print (a.byteswap(True))
print ('hex:')
print (map(hex,a))
