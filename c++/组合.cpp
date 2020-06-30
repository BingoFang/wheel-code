#include <iostream.h>

class Eye
{
public:
	void Look(void);

};

class Nose
{
public:
	void Smell(void);
};

void Eat(void)
{
	cout << "Eat a fruit!" << endl;
}

class Mouth
{
public:
	void Eat(void);
};

class Ear
{
public:
	void Listen(void);
};

//正确的组合方法，Head具备了look，smell,eat,listen这些功能。
class Head
{
public:
	void Look(void) {m_eye.Look();}
	void Smell(void){m_nose.Smell();}
	void Eat(void){m_mouth.Eat();}
	void Listen(void){m_ear.Listen();}

private:
	Eye  m_eye;
	Nose m_nose;
	Mouth m_mouth;
	Ear m_ear;
};

//设计错误
class Head:public Eye,public Nose,public Mouth,public Ear
{
	
};

int main()
{
		
}