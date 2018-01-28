#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
auto f=3.14;      //double
auto s("hello");  //const char*
auto z = new auto(9); // int*
//auto x1 = 5, x2 = 5.0, x3='r';//compile error

return 0;
}
