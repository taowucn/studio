#!/usr/bin/lua

module = {}
 
module.constant = "This is constant"
 
function module.func1()
    io.write("This is public\n")
end
 
local function func2()
    print("This is private")
end
 
function module.func3()
    func2()
end
 
return module
