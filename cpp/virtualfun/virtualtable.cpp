#include <iostream>
using namespace std;

class Base1 {
public:
            virtual void f() { cout << "Base1::f" << endl; }
            virtual void g() { cout << "Base1::g" << endl; }
            virtual void h() { cout << "Base1::h" << endl; }

};

class Base2 {
public:
            virtual void f() { cout << "Base2::f" << endl; }
            virtual void g() { cout << "Base2::g" << endl; }
            virtual void h() { cout << "Base2::h" << endl; }
};

class Base3 {
public:
            virtual void f() { cout << "Base3::f" << endl; }
            virtual void g() { cout << "Base3::g" << endl; }
            virtual void h() { cout << "Base3::h" << endl; }
};


class Derive : public Base1, public Base2, public Base3 {
public:
            virtual void f() { cout << "Derive::f" << endl; }
            virtual void g1() { cout << "Derive::g1" << endl; }
};


typedef void(*Fun)(void);

int main()
{
            Fun pFun = NULL;

            Derive d;
            int** pVtab = (int**)&d;

            //Base1's vtable
            //pFun = (Fun)*((int*)*(int*)((int*)&d+0)+0);
            pFun = (Fun)pVtab[0][0];
            pFun();
 		cout << "1..."<<endl;
            //pFun = (Fun)*((int*)*(int*)((int*)&d+0)+1);
            pFun = (Fun)pVtab[0][1];
            pFun();
 		cout << "2..."<<endl;
            //pFun = (Fun)*((int*)*(int*)((int*)&d+0)+2);
            pFun = (Fun)pVtab[0][2];
            pFun();
 		cout << "3..."<<endl;
            //Derive's vtable
            //pFun = (Fun)*((int*)*(int*)((int*)&d+0)+3);
            pFun = (Fun)pVtab[0][3];
            pFun();
 		cout << "4..."<<endl;
            //The tail of the vtable
            pFun = (Fun)pVtab[0][4];
            cout<<pFun<<endl;
 		cout << "5..."<<endl;

            //Base2's vtable
            //pFun = (Fun)*((int*)*(int*)((int*)&d+1)+0);
            pFun = (Fun)pVtab[1][0];
            pFun();
 		cout << "6..."<<endl;
            //pFun = (Fun)*((int*)*(int*)((int*)&d+1)+1);
            pFun = (Fun)pVtab[1][1];
            pFun();
 		cout << "7..."<<endl;
            pFun = (Fun)pVtab[1][2];
            pFun();
 		cout << "8..."<<endl;
            //The tail of the vtable
            pFun = (Fun)pVtab[1][3];
            cout<<pFun<<endl;
 		cout << "9..."<<endl;

            //Base3's vtable
            //pFun = (Fun)*((int*)*(int*)((int*)&d+1)+0);
            pFun = (Fun)pVtab[2][0];
            pFun();
 		cout << "10..."<<endl;
            //pFun = (Fun)*((int*)*(int*)((int*)&d+1)+1);
            pFun = (Fun)pVtab[2][1];
            pFun();
 		cout << "11..."<<endl;
            pFun = (Fun)pVtab[2][2];
            pFun();
 		cout << "12..."<<endl;
            //The tail of the vtable
            pFun = (Fun)pVtab[2][3];
            cout<<pFun<<endl;
 		cout << "13..."<<endl;
            return 0;
}