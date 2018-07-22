#!/usr/bin/lua

file = io.open("test.lua", "r")
io.input(file)
print(io.read())
io.close(file)
file = io.open("test.lua", "a")
io.output(file)
io.write("--  test.lua comment")
io.close(file)
