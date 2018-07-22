#!/usr/bin/lua

print("abc")
print("\"123\"")

string1 = "Lua"
string2 = "Tutorial"
number1 = 10
number2 = 20
print(string.format("Base %s %s", string1, string2))
date = 2; month = 1; year = 2014
print(string.format("Data %02d/%02d/%03d", date, month, year))
print(string.format("%.4f", 1/3))

print(string.byte("Lua"))
print(string.byte("Lua",3))
print(string.byte("Lua",-1))
print(string.byte("Lua",2))
print(string.byte("Lua",-2))
print(string.char(97))

s = "Deadline is 30/05/1999, firm"
date = "%d%d/%d%d/%d%d%d%d"
print(string.sub(s, string.find(s, date)))    --> 30/05/1999

print(string.gsub("hello, up-down!", "%A", "."))
