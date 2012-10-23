
// �ǽ�1 : ȭ�鿡 �簢���� �׷�����.
//		   - �⺻ �����ڰ� ������ �����϶�.
//		   - ��ü ��İ� ������ ����� ��� ����϶�.
// �ǽ�2 : 2���� Ŭ������ �簢���� �׷�����.
// �ǽ�3 : �簢�� ������ ���� Ŭ���ؾ߸� ���� �簢���� �׸� �� �ֵ��� �����϶�.

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
	int includes(int,int);

public:
	myRect(int,int,int,int);
};

myRect::myRect(int x1, int y1, int x2, int y2)
{
	upperLeftX = min(x1, x2);
	upperLeftY = min(y1, y2);
	lowerRightX = max(x1, x2);
	lowerRightY = max(y1, y2);
}

void myRect::draw(window* win)
{
	win->line(upperLeftX, upperLeftY, lowerRightX,upperLeftY);
	win->line(upperLeftX, upperLeftY, upperLeftX, lowerRightY);
	win->line(upperLeftX, lowerRightY,lowerRightX,lowerRightY);
	win->line(lowerRightX,upperLeftY, lowerRightX,lowerRightY);
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
	//myRect rect;	// (100,100)���� (200,200)�� �簢���� �׷�����.
	myRect* pRect;
	
};

myApplication::myApplication(char * apName)
: application(apName)//, rect(100,100,200,200)
{
	//pRect = new myRect(100,100,200,200);
	pRect = NULL;
}

myApplication::myApplication(char * apName, int width, int height, int x, int y)
: application(apName, width, height, x, y)//, rect(100,100,200,200)
{
	//pRect = new myRect(100,100,200,200);
	pRect = NULL;
}

myApplication::~myApplication()
{
	delete pRect;
}

void myApplication::paint()
{
	//rect.draw(this);
	if(pRect) pRect->draw(this);
}

void myApplication::mouseDown(int x,int y)
{
	static int tx, ty;
	static bool is_first = true;

	if(is_first)
	{
// 		tx = x, ty = y;
// 		is_first = false;
		if(pRect == NULL || pRect->includes(x,y))
		//if( pRect?pRect->includes(x,y):true )
		{
			tx = x, ty = y;
			is_first = false;
		}
	}
	else
	{
		if(pRect) delete pRect;
		pRect = new myRect(tx, ty, x, y);
		update();

		is_first = true;
	}
}

int mainLAF(void)
{
	myApplication theApp("RectClass", 800, 600, 50, 50);

	theApp.run();

	return 0;
}