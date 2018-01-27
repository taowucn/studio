#include <iostream>
using namespace std;

class Base {
public:
	virtual void f() { cout<<"Base b:"<<(void*)this << endl; };
};

int main(void)
{
	typedef void(*Fun)(void);
	Base b;
	Fun pFun = NULL;
	cout << "Virtual table address: " << (&b) << endl;
	cout << "The frist virtual func address: " << (int*)*(int*)(&b) << endl;
	pFun = (Fun)*((int*)*(int*)(&b));
	pFun();
	b.f();
	Base* pNull = &b;
	pNull->f();
}