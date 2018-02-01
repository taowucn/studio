#include <iostream>
using namespace std;

void demo_static_cast(void)
{
  int m = 8;
  double n = static_cast<double> (m);
  cout <<"m="<< m << ", n="<< n << endl;
}

class B
{
  public:
    int num;
};

void demo_const_cast(void)
{
  B b0;
  b0.num = 100;
  const B b1 = b0;
  cout << "b0.num=" << b0.num << ", b1.num=" << b1.num << endl;
  //b1.num = 200; compile will failed
  const_cast<B&>(b1).num = 200;
  cout << "b0.num=" << b0.num << ", b1.num=" << b1.num << endl;
}

void demo_reinterpret_cast()
{
  int m = 9;
  double n = reinterpret_cast<double &> (m);
  cout << "m=" << m << ", n=" << n << endl;
}

int main(int argc, char **argv)
{
  demo_static_cast();
  demo_const_cast();
  demo_reinterpret_cast();
  return 0;
}
