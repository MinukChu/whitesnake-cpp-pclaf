
#include "../pclaf.h"

class myApplication : public application
{
public:
	myApplication(char *);
	myApplication(char *,int,int,int,int);
	void paint();
	void mouseDown(int,int);

public:
	int oldx;
	int oldy;
	int newx;
	int newy;
	bool is_first;
};

myApplication::myApplication(char * apName)
: application(apName)
{
	oldx = 0;
	oldy = 0;
	newx = 0;
	newy = 0;
	is_first = true;
}

myApplication::myApplication(char * apName, int width, int height, int x, int y)
: application(apName, width, height, x, y)
{
	oldx = 0;
	oldy = 0;
	newx = 0;
	newy = 0;
	is_first = true;
}

void myApplication::paint()
{
	setPen(blue, dashedLine, 2);	//1번 설정하면, 계속 유지된다.
	//line(50, 50, 300, 100);
	
	// draw line.
	//line(oldx, oldy, newx, newy);
	//oldx = newx;
	//oldy = newy;

	// draw rect.
	line(oldx, oldy, newx, oldy);
	line(oldx, oldy, oldx, newy);
	line(newx, oldy, newx, newy);
	line(oldx, newy, newx, newy);

	wout << setpos(100, 100);
	wout << "hello world";
}

void myApplication::mouseDown(int x, int y)
{
	// draw line.
	/*if(is_first)
	{
		oldx = x;
		oldy = y;
		is_first = false;
	}
	newx = x;
	newy = y;

	update();*/
	//clearAndUpdate();

	// draw rect.
	if(is_first)
	{
		oldx = x;
		oldy = y;
		is_first = false;
	}
	else
	{
		newx=x;
		newy=y;
		is_first = true;
		update();
	}
}

int mainLAF(void)
{
	myApplication theApp("LineDraw", 500, 500, 150, 150);

	theApp.run();

	return 0;
}