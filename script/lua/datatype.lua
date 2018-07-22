#!/usr/bin/lua

print("1. nil ====")
tab1 = { key1 = "val1", key2 = "val2", "val3" }
for k, v in pairs(tab1) do
    print(k .. " - " .. v)
end
 
tab1.key1 = nil
for k, v in pairs(tab1) do
    print(k .. " - " .. v)
end

print("\n")
print("2. nil ====")
print(type(x))
print(type(x)==nil)
print(type(x)=="nil")

print("\n")
print("3. boolean ====")
print(type(true))
print(type(false))
print(type(nil))
if false or nil then
	print("one is ture")
else
	print("both are false")
end

print("\n")
print("4. number ====")
print(type(2))
print(type(2.2))
print(type(0.2))
print(type(2e+1))
print(type(0.2e-1))
print(type(7.826-06))

print("\n")
print("5. string ====")
html = [[
<html>
<head></head>
<body>
    <a href="http://www.sina.com/">Lua Traning</a>
</body>
</html>
]]
print(html)

print("2" + 6)
print("2" + "6")
print("2 + 6")
print("-2e2" + "6")
--print("err" + 1)
print("abc" .. 'b')
print(123 .. 428)
str="hellowold"
print(#str)

print("\n")
print("6. table ====")
a = {}
a["key"] = "value"
key = 10
a[key] = 22
a[key] = a[key] + 11
for k, v in pairs(a) do
	print(k .. ":" .. v)
end

local tb1 = {"apple", "pear", "orange"}
for k, v in pairs(tb1) do
	print("Key:", k)
end

local tb2 = {}
for i = 1, 10 do
	tb2[i] = i
end
tb2["key"] = "val"
print(tb2[1])
print(tb2["key"])
print(tb2["none"])
for k, v in pairs(tb2) do
	print(k, v)
end

print("\n")
print("7. function ====")
function factorial1(n)
    if n == 0 then
        return 1
    else
        return n * factorial1(n - 1)
    end
end
print(factorial1(5))
factorial2 = factorial1
print(factorial2(5))

function testFun(tab, fun)
	for k, v in pairs(tab) do
		print(fun(k, v));
	end
end
tab={k1="val1", k2="val2"}
testFun(tab, 
function(key, val)
	return key..":"..val
end
);
