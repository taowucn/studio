#!/usr/bin/env python

import numpy as np

print (np.char.add(['hello'],[' xyz']))
print (np.char.add(['hello', 'hi'],[' abc', ' xyz']))

print (np.char.multiply('Hello ',3))

print (np.char.center('Hello', 20,fillchar = '*'))

print (np.char.capitalize('hello'))

print (np.char.title('i like python'))

print (np.char.lower(['HELLO','WORLD']))

print (np.char.upper(['i','me']))

print (np.char.split ('i like python?'))

print (np.char.splitlines('i\nlike python?'))
print (np.char.splitlines('i\rlike python?'))

print (np.char.strip(['helloa','admin','java'],'a'))

print (np.char.join([':','-'],['hello','google']))

print (np.char.replace ('i like you', 'ou', 'uo'))

a = np.char.encode('hello', 'cp500')
print (a)

a = np.char.encode('hello', 'cp500')
print (a)
print (np.char.decode(a,'cp500'))
