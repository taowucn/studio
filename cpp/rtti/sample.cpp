#include <iostream>
#include <typeinfo>
#include <string>
using namespace std;

class Animal
{
    public:
        virtual bool equal(const Animal &other)const = 0;};

class Cat:public Animal
{
    public:
        bool equal(const Animal &other)const
        {
            cout << "Cat" << endl;
            if(const Cat *pc = dynamic_cast<const Cat*>(&other)) {
              return name_ == pc->name_;
            }
            return false;
        }
    private:
        string name_;
};

class Dog:public Animal
{
    public:
        bool equal(const Animal &other)const
        {
            cout << "Dog" << endl;
            if(const Dog *pd = dynamic_cast<const Dog*>(&other)) {
              return name_ == pd->name_;
            }
            return false;
        }
    private:
        string name_;
};

bool operator==(const Animal &a, const Animal &b)
{
    return typeid(a)==typeid(b)&&a.equal(b);
}


int main(int argc, const char *argv[])
{
    Cat c;
    Dog d;
    Animal *pa = &c;
    cout <<( *pa == c) << endl;
    cout <<( *pa == d) << endl;
    return 0;
}
