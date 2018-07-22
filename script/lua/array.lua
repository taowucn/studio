#!/usr/bin/lua

array={"abc", "def"}
for i = 0, 2 do
	print(array[i])
end

-- init
array = {}
for i=1,3 do
   array[i] = {}
      for j=1,3 do
         array[i][j] = i*j
      end
end

-- read
for i=1,3 do
   for j=1,3 do
      print(array[i][j])
   end
end
