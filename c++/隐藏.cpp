#include <iostream.h>

/*
	Derived::f(float)覆盖了Base::f(float)
	Derived::g(int)隐藏了Base::g(float),而不是重载。【重载针对于同类函数】
	Derived::h(float)隐藏了Base::h(float),而不是覆盖。【覆盖需要加virtual】
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