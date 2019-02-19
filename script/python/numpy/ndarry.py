#!/usr/bin/env python
import numpy as np

a = np.array([1, 2, 3, 4], ndmin = 2)
print "a:", a

b = np.array([[1, 2], [3, 4]])
print "b:", b

c = np.array([1, 2, 3], dtype=complex)
print "c:", c
