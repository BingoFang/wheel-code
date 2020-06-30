#include <iostream.h>

/*
	Derived::f(float)������Base::f(float)
	Derived::g(int)������Base::g(float),���������ء������������ͬ�ຯ����
	Derived::h(float)������Base::h(float),�����Ǹ��ǡ���������Ҫ��virtual��
*/
class Base
{
public:
	virtual	void f(float x){cout << "Base::f(float)" << x << endl;}
			void g(float x){cout << "Base::g(float)" << x << endl;}
			void h(float x){cout << "Base::h(float)" << x << endl;}
};

class Derived : public Base
{
public:
	virtual void f(float x){cout << "Derived::f(float)" << x << endl;}
			void g(int x){cout << "Derived::g(int)" << x << endl;}
			void h(float x){cout << "Derived::h(float)" << x << endl;}
};

void main(void)
{
	Derived d;
	Base *pb = &d;
	Derived *pd = &d;

	pb->f(3.13f);   //Derived::f(float) 3.14
	pd->f(3.14f);	//Derived::f(float) 3.14

	pb->g(3.14f);	//Base::g(float) 3.14
	pd->g(3.14f);	//Derived::g(int) 3

	pb->h(3.14f);	//Base::h(float) 3.14
	pd->h(3.14f);	//Derived::h(float) 3.14
}