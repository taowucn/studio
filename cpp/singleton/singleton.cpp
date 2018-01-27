#include <iostream>
using namespace std;

class CSingleton
{
	public:
	static CSingleton *GetInstance()
	{
		if(NULL == m_pInstance){
    			m_pInstance = new CSingleton();
			cout<<"a - 1"<<endl;
		}
		cout<<"a - 2"<<endl;
		return m_pInstance;
	}

	static void Release()	//必须，否则会导致内存泄露
	{
		if(NULL != m_pInstance)
		{
			cout<<"00000"<<endl;
			delete m_pInstance;
			m_pInstance = NULL;
		}
	}

	protected:
	CSingleton()
	{
		cout<<"#CSingleton#"<<endl;
	};
	public : static CSingleton *m_pInstance;
};


CSingleton* CSingleton::m_pInstance = NULL;

class CSingleDraw: public CSingleton
{
public:

	static CSingleDraw *GetInstance()
	{
		if(NULL == m_pInstance){
			m_pInstance = new CSingleDraw();
			cout<<"b - 1"<<endl;
		}
		cout<<"b - 2"<<endl;
		return (CSingleDraw*)m_pInstance;
	}

protected:
	CSingleDraw()
	{
		cout<<"#CSingleDraw#"<<endl;
	}
};

int main()
{
	cout<<"1-------"<<endl;
	CSingleDraw* s1 = CSingleDraw::GetInstance();
	cout<<"2-------"<<endl;
	CSingleDraw* s2 = CSingleDraw::GetInstance();
	cout<<"3-------"<<endl;
	CSingleton* s3 = CSingleton::GetInstance();
	CSingleton* s4 = CSingleton::GetInstance();
	s1->Release();
	cout<<"4-----"<<endl;
	s2->Release();
	cout<<"5-----"<<endl;
	return 0;
}

