#include <iostream>
using namespace std;

class Base {
    private:
            virtual void f() { cout << "Base::f" << endl; }
};

class Derive : public Base{
};

typedef void(*Fun)(void);

int main()
{
    Derive d;
    Fun  pFun = (Fun)*((int*)*(int*)(&d)+0);
    pFun();
}