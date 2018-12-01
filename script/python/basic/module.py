#!/usr/bin/python
#coding=utf-8
import support

support.print_func("123")

Money = 2000
def AddMoney():
   # 想改正代码就取消以下注释:
   global Money
   Money = Money + 1
 
print Money
AddMoney()
print Money

import math
 
content = dir(math)
 
print content;

