#include <iostream.h>

/*Base::f(int)与Base::f(float)相互重载，而Base::g(void)被Derived::g(void)覆盖*/
class Base
{
public:
	void f(int x){cout << "Base::f(int)" << x << endl;}
	void f(float x){cout << "Base::f(float)" << x << endl;}
	virtual void g(void){cout << "Base::g(void)" << endl;}
};

class Derived : public Base
{
public:
	virtual void g(void){cout << "Derived::g(void)" << endl;}
};

void main(void)
{
	Derived d;
	Base *pb = &d;
	pb->f(42);
	pb->f(3.14f);
	pb->g();
}