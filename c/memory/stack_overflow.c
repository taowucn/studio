#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
  char str[2];

#ifdef STACK_MEM_FREE
  /* Segament fault after free */
  char buf[4];
  free(buf);
#endif

  strcpy(str, "taowuAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
  printf("sizeof=%ld, strlen=%ld, str=%s\n",sizeof(str), strlen(str), str);

  return 0;
}
