#include <iostream.h>


/*
	������Ĺ��캯��Ӧ�����ʼ��������û���Ĺ��캯����
	���������������������Ӧ��Ϊ�顣
	�������������Ϊ�飬��ô������Ϊ��~Base
*/
class Base
{
public:
	virtual	~Base() {cout << "~Base" << endl;}
};

class Derived : public Base
{
public:
	virtual ~Derived() {cout<<"~Derived"<<endl;}   //�������������ȴ�������ո��Ƿ���ж���
};

void main(void)
{
	Base * pB = new Derived;     
	delete pB;
}

/*
	��������~Derived
			  ~Base
*/
