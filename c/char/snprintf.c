
#include <stdio.h>

char d[]="12345678abc";          //want to see  00000001abc after snprintf()

long n = 1;

int main (void)
{
  char cmd[32]="abc";
  char cmd2[32];

  snprintf (d, 9, "%08ld", n);    //minor change here.

  printf("cmd=%s\n", cmd);
  snprintf(cmd2, sizeof(cmd2), "%s %s", "123", cmd);
  printf ("cmd2=%s\n", cmd2);

  printf ("%s\n", d);

  return 0;
}

