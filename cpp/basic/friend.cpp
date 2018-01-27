
#include <iostream>

using namespace std;

class B;

class A{
    private:
        int x;
    public:
        A();
        void display(B &);
};

class C;

class B{
    private:
        int y;
        int z;
    public:
        B();
        B(int, int);
    friend void A::display(B &);//友元成员函数
    friend void display(B &);//友元函数
    friend class C;//友元类
};

class C{
    private:
        int sum;
        void calc(B &);
    public:
        C();
        void display(B &);
};

//必须在友元关系的类后进行定义
void display(B &v)//友元函数
{
    cout << v.y << " " << v.z << endl;
}

A::A()
{
    this->x = 0;
}

void A::display(B &v)//友元成员函数
{
    this->x = v.y + v.z;
    cout << this->x << endl;
}

B::B()
{
    this->y = 0;
    this->z = 0;
}

B::B(int y, int z)
{
    this->y = y;
    this->z = z;
}

C::C()
{
    sum = 0;
}

void C::display(B &v)
{
    this->calc(v);
    cout << sum << " = " << v.y << " + " << v.z << endl;
}

void C::calc(B &v)
{
    sum = v.y + v.z;
}

int main()
{
    A a;
    B b(2, 3);
    display(b);
    a.display(b);
    C c;
    c.display(b);

    return 0;
}
