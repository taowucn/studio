#include <stdlib.h>
#include <stdio.h>

char *int_to_string(int num)
{
  char tmp[10];
  char *str;
  int i = 0;
  int n = 0;
  while(num) {
    tmp[n++] = num%10 + '0';
    num = num/10;
  }
  n--;
  printf("Temp:%s\n", tmp);
  while(n >= 0){
   str[i] = tmp[n];
   i++;
   n--;
  }
  str[i]=0;
  return str;
}

int main(int argc, char **argv)
{
  int i = 1234;
  char *str;
  str = int_to_string(i);
  printf("[%d] To String [%s]\n", i, str);
  snprintf(str, 10, "%d", i);
  printf("[%d] snprintf [%s]\n", i, str);
  return 0;
}
