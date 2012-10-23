
// �ǽ� : �簢���� ũ�� ������ �����϶�.
//		  - ������ ���ľ˰��� �Լ��� ����� �� �ֵ��� Ŭ������ �����Ѵ�.
/*
void SWAP(int& x, int& y)
{
	int temp = x;
	x  = y;
	y = temp;
}

void QuickSort(int ary[], int n)
{
	if(n<=1) return;

	int last=0;

	// ù��° ����� ������ ���� ��ҵ��� ã�Ƽ�
	// ù��° ����� ������ġ���� ���ʴ�� �������� �״´�.
	for(int i=1; i<n; i++)
	{
		if(ary[i] < ary[0])
		{
			last += 1;
			SWAP(ary[i], ary[last]);
		}
	}

	// ù��° ��ҿ� ù��° ��Һ��� ���� ��ҵ��� ������ ��Ҹ� �����Ͽ�,
	// ��ü �迭�� ù��°��Һ��� ���� ��ҵ�� ū ��ҵ�� �̵���Ѵ�.
	SWAP(ary[0], ary[last]);

	// ���� ��ҵ��� ���տ��� ������ �� �۾��� �ݺ������Ѵ�.
	QuickSort(ary, last);
	// ū ��ҵ��� ���տ��� ������ �� �۾��� �ݺ������Ѵ�.
	QuickSort(ary+(last+1), n-(last+1));
}
*/

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
	int  includes(int,int);
	int  area() const;

	int getPosX() const
	{
		return upperLeftX;
	}
	int getPosY() const
	{
		return upperLeftY;
	}

public:
	myRect& operator = (const myRect&);
	bool operator > (const myRect&);
	bool operator < (const myRect&);

public:
	myRect(int,int,int,int);
	myRect(){}
};

myRect::myRect(int x1, int y1, int x2, int y2)
{
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

int myRect::area() const
{
	return((lowerRightX-upperLeftX) * (lowerRightY-upperLeftY));
}

myRect& myRect::operator = (const myRect& right)
{
	if(this != &right)
	{
		upperLeftX = right.upperLeftX;
		upperLeftY = right.upperLeftY;
		lowerRightX = right.lowerRightX;
		lowerRightY = right.lowerRightY;
	}
	return *this;
}

bool myRect::operator > (const myRect& right)
{
	return area() > right.area();
}

bool myRect::operator < (const myRect& right)
{
	return area() < right.area();
}

void swap_rect(myRect& x, myRect& y)
{
	myRect temp = x;
	x = y;
	y = temp;
}

void quicksort_rect(myRect ary[], int n)
{
	if(n<=1) return;

	int last=0;

	for(int i=1; i<n; i++)
	{
		if(ary[i] < ary[0])
		{
			last += 1;
			swap_rect(ary[i], ary[last]);
		}
	}

	swap_rect(ary[0], ary[last]);

	quicksort_rect(ary, last);
	quicksort_rect(ary+(last+1), n-(last+1));
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
	myRect rects[MAXRECTS];
	int idx;
};

myApplication::myApplication(char * apName)
: application(apName), idx(0)
{
}

myApplication::myApplication(char * apName, int width, int height, int x, int y)
: application(apName, width, height, x, y), idx(0)
{
}

myApplication::~myApplication()
{
}

void myApplication::paint()
{
	quicksort_rect(rects, idx);

	setPen(blue);

	for(int i=0; i<idx; i++)
	{
		if(i == idx-1) setPen(brightRed);

		rects[i].draw(this);
		wout << setpos(rects[i].getPosX()+3, rects[i].getPosY()+3) << i+1;
	}
}

void myApplication::mouseDown(int x,int y)
{
	if(idx==MAXRECTS) return;

	static int tx, ty;
	static bool is_first = true;

	if(is_first)
	{
		//if( idx?rects[idx-1].includes(x,y):true )
		{
			tx = x, ty = y;
			is_first = false;
		}
	}
	else
	{
		rects[idx++].moveTo(tx, ty, x, y);
		clearAndUpdate();

		is_first = true;
	}
}

int mainLAF(void)
{
	myApplication theApp("SortBySize", 800, 600, 50, 50);

	theApp.run();

	return 0;
}