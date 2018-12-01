#!/usr/bin/python
 
str = raw_input("Please input:")
print "Input:", str
str = input("Please input formula:")
print "Input result:", str

fo = open("/tmp/1.txt", "w")
fo.write("123")
fo.close()
print "Filename:", fo.name
print "Isclose:", fo.closed
print "Accesemode:", fo.mode
print "Space in end:", fo.softspace
