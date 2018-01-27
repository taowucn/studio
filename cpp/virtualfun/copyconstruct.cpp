#include <iostream>
#include <cstring>
#include <stdlib.h>
#include <stdio.h>

using namespace std;
class CA
{
public:
	CA(int b,char* cstr)
	{
		a=b;
		str=new char[b];
		strcpy(str,cstr);
	}
	CA(const CA& C)
	{
		a=C.a;
		str=new char[a];
		if(str!=0) {
			strcpy(str,C.str);
		}
	}
	void Show()
	{
		cout<<str<<endl;
	}
	~CA()
	{
		delete str;
	}

private:
	int a;
	char *str;
};

int main()
{
	CA A(10,"Hello!");
	CA B=A;
	B.Show();
	return 0;
}