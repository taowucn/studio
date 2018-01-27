#include <iostream>

using namespace std;

class A
{
public:
	A(){cout << "This is A" << endl;};
	void f1() {cout << "A::f1" << endl;};
	virtual void f2() {cout << "A::f2" << endl;};
	virtual void f3()=0;
	virtual ~A() {cout << "A going to die" << endl;};
};

class B : public A
{
public:
	B() {cout << "This is B" << endl;};
	void f1() { cout << "B::f1" << endl;};
	void f2() { cout << "B::f2" << endl;};
	void f3() { cout << "B::f3" << endl;};
	virtual ~B() {cout << "B going to die" << endl;};
};


int main(int argc, char* argv[])
{
	A *m_j=new B();
	m_j->f1();
	m_j->f2();
	m_j->f3();
	delete m_j;
	return 0;
}

