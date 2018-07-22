#!/usr/bin/lua

function foo (a)
    print("foo ret = ", a)
    return coroutine.yield(2 * a)
end
 
co = coroutine.create(function (a , b)
    print("1 thread ret = ", a, b)
    local r = foo(a + 1)
    print("2 thread ret = ", r)
    local r, s = coroutine.yield(a + b, a - b)
    print("3 thread ret = ", r, s)
    return b, "Stop thread"
end)
        
print("main", coroutine.resume(co, 1, 10)) -- true, 4
print("------")
print("main", coroutine.resume(co, "r")) -- true 11 -9
print("------")
print("main", coroutine.resume(co, "x", "y")) -- true 10 end
print("------")
print("main", coroutine.resume(co, "x", "y")) -- cannot resume dead coroutine
print("------")

