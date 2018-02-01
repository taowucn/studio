#include <iostream>
#include <typeinfo>

using namespace std;

#define OUTPUT(f)	cout << #f << "\t: " << typeid(f).name() << endl;
class BaseA {};
class DeriveA: public BaseA {};

class BaseB
{
	virtual void f(){}
};
class DeriveB: public BaseB {};

int main(int argc, char* argv[])
{
	cout << "------- Class name -------" <<endl;

	OUTPUT(BaseA);
	OUTPUT(DeriveA);
	OUTPUT(BaseB);
	OUTPUT(DeriveB);

	cout << endl << "------- Base class contain virtual function -------" <<endl;

	BaseA baseA;
	DeriveA deriveA;
	OUTPUT(baseA);
	OUTPUT(deriveA);

	BaseA* pa;
	pa = &baseA;
	OUTPUT(*pa);
	OUTPUT(pa);
	pa = &deriveA;
	OUTPUT(*pa);
	OUTPUT(pa);

	cout << endl << "-------Base class don't contain virtual function-------" <<endl;

	BaseB baseB;
	DeriveB deriveB;
	OUTPUT(baseB);
	OUTPUT(deriveB);

	BaseB* pb;
	pb = &baseB;
	OUTPUT(*pb);
	OUTPUT(pb);
	pb = &deriveB;
	OUTPUT(*pb);
	OUTPUT(pb);
}
