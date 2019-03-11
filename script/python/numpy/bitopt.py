#!/usr/bin/env python

import numpy as np

print ('13, 17 bit:')
a,b = 13,17
print (bin(a), bin(b))

print ('13, 17 bit_and:')
print (np.bitwise_and(13, 17))

print ('13, 17 bit_or:')
print (np.bitwise_or(13, 17))


print ('13 invert:')
print (np.invert(np.array([13], dtype = np.uint8)))
print (np.binary_repr(13, width = 8))


print ('10 leftshift:')
print (np.left_shift(10, 2))


print ('40 rightshift')
print (np.right_shift(40, 2))


