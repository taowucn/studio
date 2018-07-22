#!/usr/bin/lua

a = 20
b = 10
c = 15
d = 5

print("a", "b", "c", "d");
print(a, b, c, d)

-- ^/.. is right connection, others are left connection
e = (a + b) * c / d;-- ( 30 * 15 ) / 5
print("(a + b) * c / d = ",e )

e = ((a + b) * c) / d; -- (30 * 15 ) / 5
print("((a + b) * c) / d = ",e )

e = (a + b) * (c / d);-- (30) * (15/5)
print("(a + b) * (c / d) = ",e )

e = a + (b * c) / d;  -- 20 + (150/5)
print("a + (b * c) / d = ",e )
