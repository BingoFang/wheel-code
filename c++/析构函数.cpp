#include <iostream.h>


/*
	派生类的构造函数应在其初始化表里调用基类的构造函数。
	基类与派生类的析构函数应该为虚。
	如果析构函数不为虚，那么输出结果为：~Base
*/
class Base
{
public:
	virtual	~Base() {cout << "~Base" << endl;}
};

class Derived : public Base
{
public:
	virtual ~Derived() {cout<<"~Derived"<<endl;}   //如遇到语句正常却报错，检查空格是否藏有东西
};

void main(void)
{
	Base * pB = new Derived;     
	delete pB;
}

/*
	输出结果：~Derived
			  ~Base
*/
