#!/usr/bin/python
#coding=utf-8

BOARD_SIZE = 8
time = 0

def under_attack(col, queens):
   left = right = col
   for r, c in reversed(queens):
   #左右有冲突的位置的列号
       left, right = left - 1, right + 1

       if c in (left, col, right):
           return True
   return False

def solve(n):
   if n == 0:
       return [[]]

   smaller_solutions = solve(n - 1)

   return [solution+[(n,i+1)]
       for i in xrange(BOARD_SIZE)
           for solution in smaller_solutions
               if not under_attack(i+1, solution)]
for answer in solve(BOARD_SIZE):
   print answer
   time = time + 1

print "Total num:", time
