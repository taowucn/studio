
#include <iostream>
 
using namespace std;
 
class Line
{
   public:
      int getLength( void );
      Line( int len );             //Simple construct
      Line( const Line &obj);      //Copy construct
      ~Line();                     //destruct
 
   private:
      int *ptr = NULL;
};

Line::Line(int len)
{
    cout <<"Simple construct"<< endl;
    ptr = new int;
    *ptr = len;
}

#if 1
Line::Line(const Line &obj)
{
    cout <<"Copy construct"<< endl;
    ptr = new int;
    *ptr = *obj.ptr;
}
#endif

Line::~Line(void)
{
    cout <<"Destruct"<< endl;
    delete ptr;
}

int Line::getLength( void )
{
    return *ptr;
}
 
void display(Line& obj)
{
   cout << "Line: " << obj.getLength() <<endl;
}

int main(int argc, char *argv[])
{
   Line line1(10);
   cout << "1  ====" << endl;
   Line line2 = line1;
   
   cout << "2  ====" << endl;
   display(line1);
   cout << "3  ====" << endl;
   display(line2);
 
   return 0;
}
