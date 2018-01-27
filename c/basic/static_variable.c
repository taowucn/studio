#include <stdio.h>
#include <stdlib.h>

int sum(int a)
{
  int c=0;
  static int b=3;
  c+=1;
  b+=2;
  return (a + b +c);
}

int main (int argc, char ** argv)
{
  int i=0;
  int a=2;
  int *p = 0x12345678;

  printf("p[%x], p+2[%x].\n", p, p+2);
  for (i; i< 5; i++){
    printf("%d\n", sum(a));
  }
  return 0;
}
