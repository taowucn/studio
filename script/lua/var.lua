#!/usr/bin/lua

a = 5
local b = 5
function joke()
    c = 5     
    local d = 6
end

joke()
print(c,d)
do
    local a = 6
    b = 6     
    print(a,b);
end
print(a,b)

a, b, c = 0, 1
print(a, b, c)

a, b = a+1, b+1, b+2
print(a, b)

a, b, c = 0
print(a, b, c)

if(0)
then 
	print("0 is true")
end
