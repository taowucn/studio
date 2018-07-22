#!/usr/bin/lua

local newProductor

function productor()
     local i = 0
     while true do
          i = i + 1
          send(i)
     end
end

function consumer()
     while true do
          local i = receive()
          print(i)
     end
end

function receive()
     local status, value = coroutine.resume(newProductor)
     return value
end

function send(x)
     coroutine.yield(x)     -- x is send value，suspend it after return
end

--start thread
newProductor = coroutine.create(productor)
consumer()

