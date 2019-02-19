#!/usr/bin/env python

import numpy as np

print (np.char.add(['hello'],[' xyz']))
print (np.char.add(['hello', 'hi'],[' abc', ' xyz']))

print (np.char.multiply('hello ',3))

print (np.char.center('hello', 20, fillchar = '*'))

print (np.char.capitalize('hello'))

print (np.char.title('i like code'))

print (np.char.lower('HELLO'))

print (np.char.upper('hello'))

## spilte by white space by default
print (np.char.split ('i like code?'))
print (np.char.split ('www.sina.com', sep = '.'))

## support all \n \r \r\n
print (np.char.splitlines('i\nlike runoob?'))
print (np.char.splitlines('i\rlike runoob?'))

print (np.char.strip('ahello aworld','a'))

print (np.char.join(':','hello'))
print (np.char.join([':','-'],['hello','world']))

print (np.char.replace ('i like world', 'or', 'and'))

a = np.char.encode('hello', 'cp500')
print (a)
print (np.char.decode(a,'cp500'))

