#include <iostream.h>

class A
{
public:
	void Func1(void);
	void Func2(void);
};

class B:public A
{
public:
	void Func3(void);
	void Func4(void);
};


int main()
{
	B b;
	b.Func1();    //B从A继承了函数Func1
	b.Func2();	  //N从A继承了函数Func2
	b.Func3();
	b.Func4();
}