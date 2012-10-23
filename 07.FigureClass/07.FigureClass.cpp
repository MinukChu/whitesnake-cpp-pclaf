
//////////////////////////////////////////////////////////////////////////
// 1. myCircleŬ������ �ۼ� ��,
//	  ����Ŭ������ ī��, ������Ŭ������ ��(������50) �׸���.(�ִ�20��)
//   - ī��� ���� ���ؼ� �ִ� 20������ �׸��� ���ؼ���
//	   ī��� ���� �ּҸ� ��� ���� �� �ִ� ���� �θ� �ʿ��ϴ�.
//	   �̸� ���ؼ� figureŬ������ �ۼ�����.
//	 - figureŬ������ myCircle�� myRect�� �θ� Ŭ������ �����.
/*
		class figure
		class myCircle : public figure
		class myRect : public figure
		class card : public myRect

		void mouseRDown(int, int);
		win->circle(x, y, radius);
		win->rectangle(x, y, width, height);

		figure* ary[20] = new card, new myCirle
*/
// 2. ����Ŭ������ ���� ����, ���� includes()�Լ� �ϼ�.
// 3. myTriangleŬ���� �߰� ��, ���Ŭ������ �ﰢ�� �׸���.
//	  - �ﰢ���� ���� Ŭ������ �����ϱ� ���� includes()�Լ� �ۼ�.
/*
		void mouseMDown(int, int);
		point trianglePoints[] = {{x, y}, {x1, y1}, {x2, y2}};
		win->polygon(trianglePoints, 3);	// �ٰ����� �׸��� �Լ�.
*/
// 4. ī�尡 �ִ� 52����� �����ǵ��� �ϰ�, âũ�⿡ �°� ���ĵǵ��� �Ͻÿ�.
//////////////////////////////////////////////////////////////////////////

#include "../pclaf.h"
#include <cmath>

// figure class.
class figure
{
protected:
	int x;
	int y;

	color col;
	int thick;

public:
	void moveTo(int,int);
	virtual void draw(window*) = 0;
	virtual int includes(int,int) = 0;

	int getPosX() const {return x;}
	int getPosY() const {return y;}

	void setPen(color c, int t)
	{
		col = c;
		thick = t;
	}

public:
	figure(int,int);
};

figure::figure(int x, int y)
{
	moveTo(x,y);
	col = black;
	thick = 1;
}

void figure::moveTo(int x, int y)
{
	this->x = x;
	this->y = y;
}

// myCircle class.
class myCircle : public figure
{
protected:	
	int radius;

public:
	void draw(window*);
	int includes(int,int);

public:
	myCircle(int,int,int);
};

myCircle::myCircle(int x, int y, int r)
: figure(x,y), radius(r)
{
}

int myCircle::includes(int px, int py)
{
	int distanceSq = 0;
	distanceSq = (px-x)*(px-x) + (py-y)*(py-y);
	if(distanceSq <= radius*radius)
		return 1;
	else
		return 0;
}

void myCircle::draw(window* win)
{
	win->setPen(col, solidLine, thick);
	win->circle(x, y, radius);
}

// myTriangle class.
class myTriangle : public figure
{
protected:
	int length;

public:
	void draw(window*);
	int includes(int,int);

public:
	myTriangle(int,int,int);
};

myTriangle::myTriangle(int x, int y, int l)
: figure(x,y), length(l)
{
}

void myTriangle::draw(window* win)
{
	win->setPen(col, solidLine, thick);
	int dx = int(length*cos(60*3.141592/180));
	int dy = int(length*sin(60*3.141592/180));
	point trianglePoints[] = {{x, y}, {x-dx, y+dy}, {x+dx, y+dy}};
	win->polygon(trianglePoints, 3);		// �ٰ����� �׸��� �Լ�.
}

int myTriangle::includes(int px, int py)
{
	int dx = int(length*cos(60*3.141592/180));
	int dy = int(length*sin(60*3.141592/180));

	int left = x-dx;
	int right = x+dx;
	int top = y;
	int bottom = y+dy;

	if(left < px && px < right)
	{
		if(top < py && py < bottom)
		{
			double yOfx = (double)dy/dx;

			double xLength;
			if(px < x) xLength = px - left;
			else xLength = right - px;
			double yLength = bottom - py;

			if(yLength/xLength <= yOfx)
				return 1;
		}
	}

	return 0;
}

// myRect class.
class myRect : public figure
{
protected:
	int width;
	int height;

public:
	void draw(window*);
	int  includes(int,int);
	int  area() const;

public:
	myRect& operator = (const myRect&);
	bool operator > (const myRect&);
	bool operator < (const myRect&);

public:
	myRect(int,int,int,int);
};

myRect::myRect(int x, int y, int w, int h)
:figure(x,y), width(w), height(h)
{
}

void myRect::draw(window* win)
{
// 	win->line(upperLeftX, upperLeftY, lowerRightX,upperLeftY);
// 	win->line(upperLeftX, upperLeftY, upperLeftX, lowerRightY);
// 	win->line(upperLeftX, lowerRightY,lowerRightX,lowerRightY);
// 	win->line(lowerRightX,upperLeftY, lowerRightX,lowerRightY);
	win->setPen(col, solidLine, thick);
	win->rectangle(x, y, width, height);
}

int myRect::includes(int px, int py)
{
	if((x<px) && (x+width>px))
		if((y<py) && (y+height>py))
			return 1;

	return 0;
}

int myRect::area() const
{
	return(width*height);
}

myRect& myRect::operator = (const myRect& right)
{
	if(this != &right)
	{
		x = right.x;
		y = right.y;
		width = right.width;
		height = right.height;
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

class card : public myRect
{
	int suit;
	int rank;
	int faceUp;

public:
	void draw(window *);
	void flip();

public:
	card(int,int,int,int);

public:
	static const int WIDTH = 75;
	static const int HEIGHT = 100;

	enum SUIT {HEART=1, CLUB, SPADE, DIAMOND};
	enum RANK {ACE=1, JACK=11, QUEEN=12, KING=13};
	enum FACE {BACK, FRONT};
};

card::card(int x, int y, int suit, int rank)
: myRect(x, y, WIDTH, HEIGHT)
{
	this->suit = suit;
	this->rank = rank;
	this->faceUp = FRONT;
}

void card::draw(window *win)
{
	// �׵θ� �׸���.
	myRect::draw(win);

	if(faceUp == FRONT)
	{
		if(suit==HEART || suit==DIAMOND) win->setTextColor(brightRed);

		// �׸����ڵ� �߰�.
		win->wout << setpos(x+5, y+5);
		if(suit==HEART) win->wout << "��";
		else if(suit==CLUB) win->wout << "��";
		else if(suit==SPADE) win->wout << "��";
		else if(suit==DIAMOND) win->wout << "��";

		win->wout << setpos(x+23, y+5);
		if(rank==ACE) win->wout << "A";
		else if(rank==JACK) win->wout << "J";
		else if(rank==QUEEN) win->wout << "Q";
		else if(rank==KING) win->wout << "K";
		else win->wout << rank;

		if(suit==HEART || suit==DIAMOND) win->setTextColor(black);
	}
	else
	{
		win->line(x, y, x+WIDTH, y+HEIGHT);
		win->line(x+WIDTH, y, x, y+HEIGHT);
	}
}

void card::flip()
{
	faceUp = !faceUp;
}

class myApplication : public application
{
public:
	myApplication(char *);
	myApplication(char *,int,int,int,int);
	~myApplication();
	void paint();
	void mouseDown(int,int);
	void mouseRDown(int,int);
	void mouseMDown(int, int);
	void doubleClick(int,int);

public:
	static const int MAXCOUNT = 20;
	figure* figures[MAXCOUNT];
	int idx;
};

myApplication::myApplication(char * apName)
: application(apName), idx(0)
{
	for(int i=0; i<MAXCOUNT; i++) figures[i] = NULL;
}

myApplication::myApplication(char * apName, int width, int height, int x, int y)
: application(apName, width, height, x, y), idx(0)
{
	for(int i=0; i<MAXCOUNT; i++) figures[i] = NULL;
}

myApplication::~myApplication()
{
	for(int i=0; i<idx; i++)
		delete figures[i];
}

void myApplication::paint()
{
// 	for(int i=0; i<idx; i++)
// 		figures[i]->draw(this);	

	//width(), height()
	//card::WIDTH, card::HEIGHT

	int tx = 0;
	int ty = 0;

	for(int i=0; i<idx; i++)
	{
		figures[i]->moveTo(tx, ty);
		figures[i]->draw(this);

		tx += card::WIDTH + 1;

		if(tx+card::WIDTH > width())
		{
			tx = 0;
			ty += card::HEIGHT + 1;
		}
	}
}


void myApplication::mouseDown(int x,int y)
{
	if(idx < MAXCOUNT) 
	{
		figures[idx] = new card(x,y,randomInt(card::HEART, card::DIAMOND),
								randomInt(card::ACE, card::KING));
		idx++;
	};

	clearAndUpdate();
}

void myApplication::mouseRDown(int x,int y)
{
	if(idx < MAXCOUNT) 
	{
		figures[idx] = new myCircle(x,y,50);
		idx++;
	};

	clearAndUpdate();
}

void myApplication::mouseMDown(int x, int y)
{
	if(idx < MAXCOUNT) 
	{
		figures[idx] = new myTriangle(x, y, 100);
		idx++;
	};

	clearAndUpdate();
}

void myApplication::doubleClick( int x,int y)
{
	if(idx == MAXCOUNT)
	{
		for(int i=idx-1; i>=0; i--)
		{
			if(figures[i]->includes(x,y))
			{
				figures[i]->setPen(green, 3);
				break;
			}
		}
	}

	clearAndUpdate();
}

int mainLAF(void)
{
	myApplication theApp("FigureClass", 800, 600, 50, 50);

	theApp.run();

	return 0;
}