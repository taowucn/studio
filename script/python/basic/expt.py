#!/usr/bin/python
# -*- coding: UTF-8 -*-

try:
    fh = open("/tmp/1", "w")
    fh.write("This test file\n")
except IOError:
    print "Error: can not find file"
else:
    print "write ok"
    fh.close()


def temp_convert(var):
    try:
        return int(var)
    except ValueError, Argument:
        print "Args not contain num\n", Argument

temp_convert("xyz");


def mye( level ):
    if level < 1:
        raise Exception,"Invalid level!"
        # 触发异常后，后面的代码就不会再执行
try:
    mye(0)            # 触发异常
except Exception,err:
    print 1, err
else:
    print 2
