#include <iostream>
#include <typeinfo>
using namespace std;

class Base
{
  public:
    virtual void FunA() { cout << "Base: This is A" << endl;}
};

class Derived : public Base
{
  public:
    virtual void FunA() { cout << "Derived: This is A" << endl;}
    virtual void FunB() { cout << "Derived: This is B" << endl;}
};

void FunC(Base *p)
{
  Derived *dp = dynamic_cast<Derived *> (p);
  if ( dp != NULL){
    dp->FunB();
  }else{
    p->FunA();
  }
}

void FunD(Base *p)
{
  Derived *dp = NULL;
  if ( typeid(*p) == typeid(Derived)){
    dp = static_cast<Derived *>(p);
    dp->FunB();
  }else {
    p->FunA();
  }
}

int main (int argc, char ** argv)
{
  Base *bp = new Derived();
  cout << typeid(*bp).name() << ", " << typeid(bp).name() << endl;
  FunC(bp);
  FunD(bp);
  bp->FunA();

  Base *bbp = new Base();
  FunC(bbp);
  FunD(bbp);
  bbp->FunA();
  return 0;
}
