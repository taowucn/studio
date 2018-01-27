#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

int main (int argc, char ** argv)
{
  char *ptr=NULL;
  int len=0;
  int size=0;
  ptr = (char *)malloc(0);
  len = strlen(ptr);
  size = sizeof(ptr);
  if (ptr == NULL) {
    cout << "null pointer" << endl;
  }else {
    cout << "valid pointer" << endl;
  }
  cout << "len=" << len << ", size="<< size << endl;
  return 0;
}
