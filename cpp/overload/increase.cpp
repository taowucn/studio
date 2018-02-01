#include <iostream>
using namespace std;

class Time
{
   private:
      int hours;
      int minutes;
   public:

      Time(){
         hours = 0;
         minutes = 0;
         cout << "Construct 0" << endl;
      }
      Time(int h, int m){
         hours = h;
         minutes = m;
         cout << "Construct param" << endl;
      }
      ~ Time(){
          cout << "Destruct" << endl;
      }

      void displayTime()
      {
         cout << "H: " << hours << " M:" << minutes <<endl;
      }
      /* overload prefix */
      Time &operator++ ()
      {
         ++minutes;
         if(minutes >= 60)
         {
            ++hours;
            minutes -= 60;
         }
         //return Time(hours, minutes);
      }

      /* overload suffix */
      Time &operator++( int )
      {
         //Time T(hours, minutes);
         ++minutes;
         if(minutes >= 60)
         {
            ++hours;
            minutes -= 60;
         }

         //return T;
      }
};

int main()
{
   Time T1(11, 59), T2(10,40);

   ++T1;
   T1.displayTime();
   ++T1;
   T1.displayTime();

   T2++;
   T2.displayTime();
   T2++;
   T2.displayTime();
   return 0;
}
