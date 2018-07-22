#!/usr/bin/lua

-- Meta class
Rectangle = {area = 0, length = 0, breadth = 0}

-- father new
function Rectangle:new (o,length,breadth)
  o = o or {}
  setmetatable(o, self)
  self.__index = self
  self.length = length or 0
  self.breadth = breadth or 0
  self.area = length*breadth;
  return o
end

-- chirdren printArea
function Rectangle:printArea ()
  print("Rentagle Area: ",self.area)
end
