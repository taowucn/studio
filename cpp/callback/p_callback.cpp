#include <stdio.h>

typedef void(*lpFunc) (void*, char*);

void GetCallBack(void * lpVoid, lpFunc callback)
{
       callback(lpVoid, "test");
}

class A{
public:
       A(){};

       void outName(char szAlarm[]){
              printf("my name is %s \n", szAlarm);
       }

       static void fCallback(void *lpVoid, char szAlarm[]){
              A * p = (A*)lpVoid;
              p->outName(szAlarm);
       }

       void Test(){
              GetCallBack(this, fCallback);
       }
};

int main(void)
{
       A a;
       a.Test();
}
