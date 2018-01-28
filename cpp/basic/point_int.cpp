#include <iostream>
#include <string>
#include <stdio.h>

using namespace std;

int main (int argc, char ** argv)
{
  int *pa = NULL;
  int *pb = pa + 15;
  printf ("%p\n", pa);
  printf ("%p\n", pb);
  return 0;
}
