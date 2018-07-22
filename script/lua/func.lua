#!/usr/bin/lua

function max(num1, num2)
	if (num1 > num2) then
		result = num1;
	else
		result = num2;
	end
	return result;
end
print("max:", max(1, 2))
print("max:", max(100, 1))

myprint = function(param)
	print("This print - ##", param, "##")
end
function add(num1, num2, pfunc)
	result = num1 + num2
	pfunc(result)
end
myprint(10)
add(2, 5, myprint)

s, e = string.find("123abc", "abc")
print(s, e)

function maximum (a)
    local mi = 1
    local m = a[mi]
    for i, val in ipairs(a) do
       if val > m then
           mi = i
           m = val
       end
    end
    return m, mi
end

print(maximum({8,10,23,12,5}))


function add(...)  
local s = 0  
  for i, v in ipairs{...} do  
    s = s + v  
  end  
  return s  
end  
print(add(3,4,5,6,7))  --->25


function average(...)
   result = 0
   local arg={...}    --> arg is table
   for i,v in ipairs(arg) do
      result = result + v
   end
   print("Input param: " .. #arg .. " num")
   return result/#arg
end
print("AVG", average(10,5,3,4,5,6))


function fwrite(fmt, ...)  ---> fixed param fmt
    return io.write(string.format(fmt, ...))  
end
fwrite("runoob\n")       --->fmt = "runoob", 
fwrite("%d%d\n", 1, 2)   --->fmt = "%d%d",

do  
    function foo(...)  
        for i = 1, select('#', ...) do  -->get param total num
            local arg = select(i, ...); -->get arg one by one
            print("arg", arg);  
        end  
    end  
  
    foo(1, 2, 3, 4);  
end
