
// 실습 : 사각형 이미지를 20개까지 유지되도록 코드를 수정하라.
//		  - 포인터 방식과, 객체 방식을 모두 적용시켜 보자.

#include "../pclaf.h"

class myRect
{
private:
	int upperLeftX;
	int upperLeftY;
	int lowerRightX;
	int lowerRightY;

public:
	void draw(window*);
	void moveTo(int,int,int,int);
	int includes(int,int);


public:
	myRect(int,int,int,int);
	myRect(){}
};

myRect::myRect(int x1, int y1, int x2, int y2)
{
// 	upperLeftX = min(x1, x2);
// 	upperLeftY = min(y1, y2);
// 	lowerRightX = max(x1, x2);
// 	lowerRightY = max(y1, y2);
	moveTo(x1,y1,x2,y2);
}

void myRect::draw(window* win)
{
	win->line(upperLeftX, upperLeftY, lowerRightX,upperLeftY);
	win->line(upperLeftX, upperLeftY, upperLeftX, lowerRightY);
	win->line(upperLeftX, lowerRightY,lowerRightX,lowerRightY);
	win->line(lowerRightX,upperLeftY, lowerRightX,lowerRightY);
}

void myRect::moveTo(int x1, int y1, int x2, int y2)
{
	upperLeftX = min(x1, x2);
	upperLeftY = min(y1, y2);
	lowerRightX = max(x1, x2);
	lowerRightY = max(y1, y2);
}

int myRect::includes(int x, int y)
{
	if((upperLeftX<x) && (lowerRightX>x))
		if((upperLeftY<y) && (lowerRightY>y))
			return 1;

	return 0;
}

class myApplication : public application
{
public:
	myApplication(char *);
	myApplication(char *,int,int,int,int);
	~myApplication();
	void paint();
	void mouseDown(int,int);

public:
	const static int MAXRECTS = 20;
	//myRect* rects[MAXRECTS];
	myRect rects[MAXRECTS];
	int idx;
};

myApplication::myApplication(char * apName)
: application(apName), idx(0)
{
	//for(int i=0; i<MAXRECTS; i++) rects[i] = NULL;
}

myApplication::myApplication(char * apName, int width, int height, int x, int y)
: application(apName, width, height, x, y), idx(0)
{
	//for(int i=0; i<MAXRECTS; i++) rects[i] = NULL;
}

myApplication::~myApplication()
{
// 	for(int i=0; i<idx; i++)
// 		delete rects[i];
}

void myApplication::paint()
{
	for(int i=0; i<idx; i++)
		//rects[i]->draw(this);
		rects[i].draw(this);
}

void myApplication::mouseDown(int x,int y)
{
	if(idx==MAXRECTS) return;

	static int tx, ty;
	static bool is_first = true;

	if(is_first)
	{
		//if( idx?rects[idx-1]->includes(x,y):true )
		if( idx?rects[idx-1].includes(x,y):true )
		{
			tx = x, ty = y;
			is_first = false;
		}
	}
	else
	{
		//rects[idx++] = new myRect(tx, ty, x, y);
		rects[idx++].moveTo(tx, ty, x, y);
		clearAndUpdate();

		is_first = true;
	}
}

int mainLAF(void)
{
	myApplication theApp("ClearAndUpdate", 800, 600, 50, 50);

	theApp.run();

	return 0;
}