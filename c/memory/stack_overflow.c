#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
  char str[2];
  strcpy(str, "taowuAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
  printf("sizeof=%d, strlen=%d, str=%s\n",sizeof(str), strlen(str), str);

  return 0;
}
