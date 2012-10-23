
#include "../pclaf.h"

class myApplication : public application
{
public:
	myApplication(char *);
	myApplication(char *,int,int,int,int);
	void paint();
};

myApplication::myApplication(char * apName)
: application(apName)
{

}

myApplication::myApplication(char * apName, int width, int height, int x, int y)
: application(apName, width, height, x, y)
{

}

void myApplication::paint()
{
	wout << setpos(100, 100);
	wout << "hello world";
}

int mainLAF(void)
{
	//myApplication theApp("CreateWindow");
	myApplication theApp("CreateWindow", 500, 500, 150, 150);

	theApp.run();

	return 0;
}