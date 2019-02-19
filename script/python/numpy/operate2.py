#!/usr/bin/env python

import numpy as np

x = np.array([[1], [2], [3]])
y = np.array([4, 5, 6])

# 对 y 广播 x
b = np.broadcast(x,y)
# 它拥有 iterator 属性，基于自身组件的迭代器元组

print ('y broadcast x:')
r,c = b.iters

# Python3.x 为 next(context) ，Python2.x 为 context.next()
print (next(r), next(c))
print (next(r), next(c))
print ('\n')
# shape 属性返回广播对象的形状

print ('shape:')
print (b.shape)
print ('\n')
# 手动使用 broadcast 将 x 与 y 相加
b = np.broadcast(x,y)
c = np.empty(b.shape)
print ('broadcast x+y:')
print (c.shape)
print ('\n')
c.flat = [u + v for (u,v) in b]
print ('flat:')
print (c)
print ('\n')
# 获得了和 NumPy 内建的广播支持相同的结果
print ('x+y:')
print (x + y)


print ('\n## broadcast_to ##')
a = np.arange(4).reshape(1,4)
print ('Org:')
print (a)
print ('\n')
print ('broadcast_to:')
print (np.broadcast_to(a,(4,4)))


print ('\n## expand_dims ##')
x = np.array(([1,2],[3,4]))
print ('x:')
print (x)
y = np.expand_dims(x, axis = 0)
print ('y:')
print (y)
print ('x, y shape:')
print (x.shape, y.shape)
# 在位置 1 插入轴
y = np.expand_dims(x, axis = 1)
print ('Insert axis.1, after y:')
print (y)
print ('x.ndim and y.ndim:')
print (x.ndim,y.ndim)
print ('x.shape and y.shape:')
print (x.shape, y.shape)


print ('\n## squeeze ##')
x = np.arange(9).reshape(1,3,3)
print ('x:')
print (x)
y = np.squeeze(x)
print ('y:')
print (y)
print ('x, y shape')
print (x.shape, y.shape)


print ('\n## concatenate ##')
a = np.array([[1,2],[3,4]])
print ('first:')
print (a)
b = np.array([[5,6],[7,8]])
print ('sencond:')
print (b)
print ('axis.0 concat:')
print (np.concatenate((a,b)))
print ('axis.1 concat:')
print (np.concatenate((a,b),axis = 1))


print ('\n## stack ##')
a = np.array([[1,2],[3,4]])
print ('first:')
print (a)
b = np.array([[5,6],[7,8]])
print ('second:')
print (b)
print ('axis.0:')
print (np.stack((a,b),0))
print ('axis.1:')
print (np.stack((a,b),1))


print ('\n## hstack##')
print ('first:')
print (a)
b = np.array([[5,6],[7,8]])
print ('second:')
print (b)
print ('hstack:')
c = np.hstack((a,b))
print (c)



print ('\n## vstack##')
print ('first:')
a = np.array([[1,2],[3,4]])
print ('first:')
print (a)
b = np.array([[5,6],[7,8]])
print ('second:')
print (b)
print ('vstack:')
c = np.vstack((a,b))
print (c)
