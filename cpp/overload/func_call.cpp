#include <iostream>
using namespace std;

class Distance
{
   private:
      int feet;
      int inches;
   public:

      Distance(){
         feet = 0;
         inches = 0;
         cout << "Construct 0" << endl;
      }
      Distance(int f, int i){
         feet = f;
         inches = i;
         cout << "Construct param" << endl;
      }
      Distance &operator()(int a, int b, int c)
      {
         //Distance D;
         feet = a + c + 10;
         inches = b + c + 100 ;
         //return D;
      }

      void displayDistance()
      {
         cout << "F: " << feet <<  " I:" <<  inches << endl;
      }
};

int main()
{
   Distance D1(11, 10), D2;

   cout << "First Distance : ";
   D1.displayDistance();

    cout << "Pointer D2: " << &D2 << endl;
   D2 = D1(10, 10, 10); // invoke operator()
   cout << "Second Distance :";
   D2.displayDistance();
   cout << "Pointer D2: " << &D2 << endl;

   return 0;
}
