#include <iostream.h>

/*Base::f(int)��Base::f(float)�໥���أ���Base::g(void)��Derived::g(void)����*/
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