
#include <iostream>

using namespace std;

class Test1
{
public:
    Test1(int n)
    {
        num=n;
    }//普通构造函数
private:
    int num;
};
class Test2
{
public:
    explicit Test2(int n)
    {
        num=n;
    }//explicit(显式)构造函数
private:
    int num;
};

int main(int argc, char *arg[])
{
    Test1 t1=12;

    //Test2 t2=12;// compile error
    Test2 t2(12);
    return 0;
}
