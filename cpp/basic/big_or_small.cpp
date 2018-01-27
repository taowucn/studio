#include <iostream>

using namespace std;

typedef union _mm {
  unsigned short a;
  unsigned int b;
} mm;

bool isbig1(void)
{
  int i = 0x12345678;
  char j = *(char *)&i;
  if (j == 0x12){
    return true;
  }
  return false;
}

int main (int argc, char ** argv)
{
  mm i;
  i.b = 0x12345678;
  cout <<hex<< "b=" << (int)i.a << endl;
  cout << "is big endian=" << isbig1() << endl;
  return 0;
}
