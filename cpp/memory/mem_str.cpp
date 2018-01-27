#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
using namespace std;

char *GetMemory(void)
{
    //char p[]="hello";
    char *p = (char *)malloc (10);
    strcpy(p, "abc");
    return p;
}
int main()
{
    char *str=NULL;
    str=GetMemory();
    cout << str << endl;
    return 0;
}
