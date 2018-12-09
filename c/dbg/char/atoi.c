#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv)
{
  int sum = 0;
  int i = 0;
  char str[6] = {'1', '2', '3', '4', '5', 0};

  while(str[i]) {
   sum = sum*10 + (str[i] - '0');
   i++;
  }
  printf("[%s] atoi [%d]\n", str, sum);
  return 0;
}

