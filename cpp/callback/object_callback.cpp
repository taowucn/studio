#include <stdio.h>
#include <stdlib.h>

typedef  void (* pFun)  (void * , char * );

void GetCallback (void *object, pFun callback)
{
	callback(object, "taowu");
};

class Back
{
public:
	Back (){};

	void putName (char * name){
		printf ("My name is %s\n", name);
	}

	static void fCallBack (void *pf, char *ch){
		Back * p = (Back *)pf;
		p->putName(ch);
	}

	void registerMe(){
		GetCallback(this, fCallBack);
	}
};


int main(int argv, char ** argc)
{
	Back b;
	b.registerMe();
	return 0;
}
