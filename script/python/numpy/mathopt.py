#!/usr/bin/env python

import numpy as np

a = np.array([0,30,45,60,90])
print ('sin:')
print (np.sin(a*np.pi/180))
print ('cos:')
print (np.cos(a*np.pi/180))
print ('tan:')
print (np.tan(a*np.pi/180))


a = np.array([0,30,45,60,90])
print ('sin:')
sin = np.sin(a*np.pi/180)
print (sin)

print ('arcsin:')
inv = np.arcsin(sin)
print (inv)

print ('degrees:')
print (np.degrees(inv))

print ('arccos, arctan:')
cos = np.cos(a*np.pi/180)
print (cos)

print ('arcos')
inv = np.arccos(cos)
print (inv)

print ('tan:')
tan = np.tan(a*np.pi/180)
print (tan)

print ('arctan')
inv = np.arctan(tan)
print (inv)

print ('degree:')
print (np.degrees(inv))


print ('=======')
a = np.array([1.0,5.55,  123,  0.567,  25.532])
print ('Orig:')
print (a)
print ('After around:')
print (np.around(a))
print (np.around(a, decimals = 1))
print (np.around(a, decimals = -1))


a = np.array([-1.7,  1.5,  -0.2,  0.6,  10])
print ('Orig:')
print (a)
print ('After floor:')
print (np.floor(a))
print ('After ceil:')
print (np.ceil(a))
