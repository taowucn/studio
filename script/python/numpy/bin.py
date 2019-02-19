#!/usr/bin/env python

import numpy as np

print ('13, 17 bin format:')
a, b = 13, 17
print (bin(a), bin(b))
print ('13, 17 bit_and:')
print (np.bitwise_and(13, 17))
print ('13, 17 bit_or:')
print (np.bitwise_or(13, 17))


print ('\n13 bit invert:')
print (np.invert(np.array([13], dtype = np.uint8)))
print ('13 bin:')
print (np.binary_repr(13, width = 8))
print ('242 bin:')
print (np.binary_repr(242, width = 8))


print ('\n10 left_shift 2:')
print (np.left_shift(10, 2))
print ('\n40 right_shift 2:')
print (np.right_shift(40, 2))

