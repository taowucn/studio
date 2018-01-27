#include <iostream>

using namespace std;

typedef void (*pFun)(int a, int b);

void execFun(int a, int b, pFun p)
{
  p(a, b);
}

void add(int a, int b)
{
  cout << a + b << endl;
}

int main (int argc, char ** argv)
{
  pFun pfun;
  pfun = add;
  execFun( 1, 2, pfun);
}
